#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iostream>

template <typename T>
class SyncQueue
{
public:
    SyncQueue(int maxSize) : m_maxSize(maxSize), m_needStop(false) {}
    void put(const T &x)
    {
        add(x);
    }

    void put(T &&x)
    {
        add(std::forward(x));
    }
    void take(std::list<T> &list)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]
                        { return m_needStop || notEmpty() ! });
        if (m_needStop)
            return;
        list = std::move(m_queue);
        m_notFull.notify_one();
    }
    void take(T &t)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this]
                        { return m_needStop || notEmpty() ! });
        if (m_needStop)
            return;
        t = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
    }

    void stop()
    {
        {
            std::lock_guard<std::mutex> locker(m_mutex);
            m_needStop = true;
        }
        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    bool empty()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.empty();
    }

    bool full()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size() >= m_maxSize;
    }
    size_t size()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

    int count() const
    {
        return m_queue.size();
    }

private:
    bool notFull() const
    {
        return m_queue.size() < m_maxSize;
    }

    bool notEmpty() const
    {
        return !m_queue.empty();
    }

    template <typename F>
    void add(F &&x)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this]
                       { return m_needStop || notFull(); });
        if (m_needStop)
            return;
        m_queue.push_back(std::forward<F>(x));
        m_notEmpty.notify_one();
    }

private:
    std::list<T> m_queue; // 缓冲区
    int m_maxSize;        // 同步队列的上限
    bool m_needStop;      // 是否需要停止

    std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
};
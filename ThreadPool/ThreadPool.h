#pragma once
#include <atomic>
#include <functional>
#include <memory>
#include "SyncQueue.h"

class ThreadPool
{
public:
    using Task = std::function<void()>;
    ThreadPool(int numsThreads = std::thread::hardware_concurrency()) : m_queue(MAX_TASK_COUNT)
    {
        start(numsThreads);
    }
    ~ThreadPool()
    {
        stop();
    }
    void stop()
    {
        std::call_once(m_flag, [this]()
                       { this->stopThreadGroup(); });
    }
    void addTask(Task &&task)
    {
        m_queue.put(std::forward<Task>(task));
    }
    void addTask(const Task &task)
    {
        m_queue.put(task);
    }

private:
    void start(int numThreads)
    {
        m_running = true;
        for (int i = 0; i < numThreads; i++)
        {
            m_threadGroup.push_back(
                std::make_shared<std::thread>(&ThreadPool::runInThread, this));
        }
    }

    void runInThread()
    {
        while (m_running)
        {
            std::list<Task> list;
            m_queue.take(list);
            for (auto &task : list)
            {
                if (!m_running)
                    return;
                task();
            }
        }
    }

    void stopThreadGroup()
    {
        m_queue.stop();
        m_running = false;
        for (auto thread : m_threadGroup)
        {
            if (thread)
                thread->join();
        }
        m_threadGroup.clear();
    }

private:
    std::list<std::shared_ptr<std::thread>> m_threadGroup;
    SyncQueue<Task> m_queue;
    std::atomic_bool m_running;
    static const int MAX_TASK_COUNT = 100;
    std::once_flag m_flag;
};
#ifndef DESIGNMODE_SINGLETON_H
#define DESIGNMODE_SINGLETON_H

#include <mutex>

template<typename T>
class Singleton
{
public:
    template<typename... Args>
    static T* instance(Args&&... args)
    {
        if(m_instance == nullptr)
        {
            m_instance = new T(std::forward<Args>(args)...);
        }
        return m_instance;
    }

    static T *getInstance()
    {
        if (m_instance == nullptr)
            throw std::logic_error("instance is not init!!!");
        return m_instance;
    }

    Singleton() = delete;

    virtual ~Singleton() = delete;

    Singleton(const Singleton &) = delete;

    Singleton &operator=(const Singleton &) = delete;

private:
    static T *m_instance;
};

template<typename T> T *Singleton<T>::m_instance = nullptr;

#endif //DESIGNMODE_SINGLETON_H

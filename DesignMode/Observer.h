#ifndef DESIGNMODE_OBSERVER_H
#define DESIGNMODE_OBSERVER_H
#include <iostream>
#include <string>
#include <functional>
#include <map>
class NonCopyable
{
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

template<typename Func>
class Events:NonCopyable
{
public:
    Events() = default;
    ~Events() = default;
    int Connect(Func&&f)
    {
        return assgin(f);
    }
    void disconnect(int key)
    {
        m_connections.erase(key);
    }

    template<class... Args>
    void notify(Args&&... args)
    {
        for(auto& it:m_connections)
            it.second(std::forward<Args>(args)...);
    }
private:
    template<typename F>
    int assgin(F&&f)
    {
        int k = m_observerID++;
        m_connections.emplace(k,std::forward<F>(f));
        return k;
    }

    int m_observerID = 0;
    std::map<int,Func>m_connections;
};





#endif //DESIGNMODE_OBSERVER_H

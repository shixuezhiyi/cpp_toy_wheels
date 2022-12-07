#ifndef DESIGNMODE_OBJECTPOOL_H
#define DESIGNMODE_OBJECTPOOL_H

#include <string>
#include <functional>
#include <memory>
#include <map>

template<typename T>
class ObjectPool
{
private:
    template<typename... Args>
    using constructor = std::function<std::shared_ptr<T>(Args...)>;
public:
    ObjectPool() = default;

    ObjectPool(const ObjectPool &) = delete;

    ObjectPool &operator=(const ObjectPool &) = delete;

    template<typename... Args>
    void init(size_t nums, Args &&... args)
    {
        std::string constructName = typeid(constructor<Args...>).name();
        for (auto i = 0; i < nums; i++)
        {
            m_objects.emplace(constructName,
                              std::shared_ptr<T>(new T(std::forward<Args>(args)...), [this, constructName](T *p)
                              {
                                  m_objects.emplace(std::move(constructName), std::shared_ptr<T>(p));
                              }));
        }
    }

    template<typename... Args>
    std::shared_ptr<T> get()
    {
        std::string name = typeid(constructor<Args...>).name();
        auto range = m_objects.equal_range(name);
        for (auto it = range.first; it != range.second; it++)
        {
            auto ptr = it->second;
            m_objects.erase(it);
            return ptr;
        }
        return nullptr;
    }

private:
    std::multimap<std::string, std::shared_ptr<T>> m_objects;

};


#endif //DESIGNMODE_OBJECTPOOL_H

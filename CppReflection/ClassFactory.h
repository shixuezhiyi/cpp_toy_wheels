//
// Created by 滕逸青 on 2022/11/23.
//

#ifndef CPPREFLECTION_CLASSFACTORY_H
#define CPPREFLECTION_CLASSFACTORY_H

#include <map>
#include <string>
#include <iostream>
#include "ClassField.h"
#include "ClassMethod.h"
#include <functional>

using std::string;
using std::map;
using std::cout;
using std::endl;

class ClassFactory;

class ReflectObject
{
private:
    string m_className;
public:
    explicit ReflectObject(const string &className = "") : m_className(className)
    {}


    void setClassName(const string &name)
    {
        m_className = name;
    }

    int getFieldCount();

    template<typename T>
    void get(const string &name, T &value);

    template<typename T>
    void set(const string &name, const T &value);

    template<typename R = void, typename ...Args>
    R call(const string &name, Args... args);

    virtual ~ReflectObject() = default;
};

class ClassFactory
{
public:
    typedef ReflectObject *(*createObjectMethod)();

private:


    ClassFactory() = default;

    ~ClassFactory() = default;

    map<std::string, createObjectMethod> m_classMap;
    map<std::string, std::map<std::string, ClassField *>> m_classFieldMap;
    map<string, map<string, ClassMethod *>> m_classMethodMap;
public:

    ClassFactory &operator=(const ClassFactory &) = delete;

    ClassFactory(const ClassFactory &) = delete;

    void registerClass(const string &className, createObjectMethod method)
    {
        std::cout << "register Class: " << className << std::endl;
        m_classMap[className] = method;
    }

    void registerClassMethod(const string &className, const string &methodName, uintptr_t method)
    {
        std::cout << "register Class Method: " << className << "." << methodName << std::endl;
        m_classMethodMap[className][methodName] = new ClassMethod(methodName, method);
    }

    void registerClassField(const string &className, size_t offset, const string &name, const string &type)
    {
        std::cout << "register Class Field: " << className << "." << name << std::endl;
        m_classFieldMap[className][name] = new ClassField(offset, name, type);
    }



    static ClassFactory &getInstance()
    {
        static ClassFactory instance;
        return instance;
    }

    ReflectObject *getClass(const std::string &className)
    {
        if (auto it = m_classMap.find(className);it != m_classMap.end())
        {
            return it->second();
        }
        return nullptr;
    }


    ClassField *getClassField(const string &className, const string &fieldName)
    {
        if (auto it = m_classFieldMap.find(className);it != m_classFieldMap.end())
        {
            if (auto it2 = it->second.find(fieldName);it2 != it->second.end())
            {
                return it2->second;
            }
        }
        return nullptr;
    }

    ClassMethod *getClassMethod(const string &className, const string &methodName)
    {
        if (auto it = m_classMethodMap.find(className);it != m_classMethodMap.end())
        {
            if (auto it2 = it->second.find(methodName);it2 != it->second.end())
            {
                return it2->second;
            }
        }
        return nullptr;
    }

    int getFieldCount(const string &className)
    {
        if (auto it = m_classFieldMap.find(className);it != m_classFieldMap.end())
        {
            return it->second.size();
        }
        return 0;
    }

    int getMethodCount(const string &className)
    {
        if (auto it = m_classMethodMap.find(className);it != m_classMethodMap.end())
        {
            return it->second.size();
        }
        return 0;
    }
};

int ReflectObject::getFieldCount()
{
    return ClassFactory::getInstance().getFieldCount(m_className);
}

template<typename T>
void ReflectObject::get(const string &name, T &value)
{
    auto offset = ClassFactory::getInstance().getClassField(m_className, name)->getOffset();
    value = *((T *) (((size_t) this) + offset));
}

template<typename T>
void ReflectObject::set(const string &name, const T &value)
{
    auto offset = ClassFactory::getInstance().getClassField(m_className, name)->getOffset();
    *((T *) (((size_t) this) + offset)) = value;
}

template<typename R, typename ...Args>
R ReflectObject::call(const string &name, Args... args)
{
    auto method = ClassFactory::getInstance().getClassMethod(m_className, name)->getMethod();
    typedef std::function<R(decltype(this), Args...)> methodType;
    return (*((methodType *) method))(this, args...);
}

#endif //CPPREFLECTION_CLASSFACTORY_H

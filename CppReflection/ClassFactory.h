//
// Created by 滕逸青 on 2022/11/23.
//

#ifndef CPPREFLECTION_CLASSFACTORY_H
#define CPPREFLECTION_CLASSFACTORY_H

#include <map>
#include <string>
#include <iostream>
#include "ClassField.h"

using std::string;
using std::map;
using std::cout;
using std::endl;
class ClassFactory;

class Object
{
private:
    string m_className;
public:
    explicit Object(const string &className = "") : m_className(className)
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

    virtual ~Object() = default;
};

class ClassFactory
{
public:
    typedef Object *(*createObjectMethod)();

private:


    ClassFactory() = default;

    ~ClassFactory() = default;

    map<std::string, createObjectMethod> m_classMap;
    map<std::string, std::map<std::string, ClassField *>> m_classFieldMap;
public:

    ClassFactory &operator=(const ClassFactory &) = delete;

    ClassFactory(const ClassFactory &) = delete;

    void registerClass(const string &className, createObjectMethod method)
    {
        std::cout << "register Class: " << className << std::endl;
        m_classMap[className] = method;
    }

    void registerClassField(const string &className, size_t offset, const string &name, const string &type)
    {
        std::cout << "register Class Field: " << className << "." << name << std::endl;
        m_classFieldMap[className][name] = new ClassField(offset, name, type);
    }

    static void showRegister()
    {
        cout<<"Class Register: "<<endl;
        for(auto& it:getInstance().m_classMap)
        {
            cout<< it.first<<": "<<endl;
            for(auto &it2:getInstance().m_classFieldMap[it.first])
            {
                cout<<it2.second->getType()<<" "<<it2.second->getName();
                cout<<"   OFFSET: "<<it2.second->getOffset()<<endl;
            }
            cout<<"-------------------------------"<<endl;
        }
    }

    static ClassFactory &getInstance()
    {
        static ClassFactory instance;
        return instance;
    }

    Object *createClass(const std::string &className)
    {
        if (auto it = m_classMap.find(className);it != m_classMap.end())
        {
            return it->second();
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
};

int Object::getFieldCount()
{
    return ClassFactory::getInstance().getFieldCount(m_className);
}

template<typename T>
void Object::get(const string &name, T &value)
{
    auto offset = ClassFactory::getInstance().getClassField(m_className, name)->getOffset();
    value = *((T *) (((size_t) this) + offset));
}

template<typename T>
void Object::set(const string &name, const T &value)
{
    auto offset = ClassFactory::getInstance().getClassField(m_className, name)->getOffset();
    *((T *) (((size_t) this) + offset)) = value;
}

#endif //CPPREFLECTION_CLASSFACTORY_H

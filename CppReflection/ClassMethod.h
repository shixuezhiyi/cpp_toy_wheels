//
// Created by 滕逸青 on 2022/11/23.
//

#ifndef CPPREFLECTION_CLASSMETHOD_H
#define CPPREFLECTION_CLASSMETHOD_H

class ClassMethod
{
public:
    ClassMethod() : m_name(""), m_method(0)
    {}

    ClassMethod(const string &name, uintptr_t method) : m_name(name), m_method(method)
    {}

    ~ClassMethod()
    {}

    const string &getName()
    {
        return m_name;
    }

    uintptr_t getMethod()
    {
        return m_method;
    }

private:
    string m_name;
    uintptr_t m_method;
};

#endif //CPPREFLECTION_CLASSMETHOD_H

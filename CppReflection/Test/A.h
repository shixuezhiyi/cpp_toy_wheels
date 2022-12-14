//
// Created by 滕逸青 on 2022/11/22.
//
#ifndef CPPREFLECTION_A_H
#define CPPREFLECTION_A_H

#include <string>
#include <iostream>
#include "../ClassFactory.h"
#include "../ClassRegister.h"

using std::cout;
using std::endl;

class A : public ReflectObject
{
public:
    A(int age = 1, const std::string &name = "dh") : m_age(age), m_name(name)
    {}
    ~A() = default;

    void f1()
    {
        std::cout << "A:f1()" << std::endl;
    }

    int f2(int a)
    {
        std::cout << "A:f2(" << a << ")" << std::endl;
        return  a;
    }


public:
    int m_age;
    std::string m_name;
};

REGISTER_CLASS(A);
REGISTER_CLASS_Field(A,m_age,int);
REGISTER_CLASS_METHOD(A,f1,void);
REGISTER_CLASS_METHOD(A,f2,int,int);
#endif //CPPREFLECTION_A_H

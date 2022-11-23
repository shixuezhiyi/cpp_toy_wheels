//
// Created by 滕逸青 on 2022/11/23.
//
#pragma once

#include <functional>

using std::function;

#include "ClassFactory.h"

#define offset(className, fieldName) \
    (size_t)(&(((className*)0) -> fieldName)) \


class ClassRegister
{
public:
    ClassRegister(const string &className, ClassFactory::createObjectMethod method)
    {
        ClassFactory::getInstance().registerClass(className, method);
    }

    ClassRegister(const string &className, size_t offset, const string &name, const string &type)
    {
        ClassFactory::getInstance().registerClassField(className, offset, name, type);
    }

    ClassRegister(const string &className, const string &name, uintptr_t method)
    {
        ClassFactory::getInstance().registerClassMethod(className, name, method);
    }
};

#define REGISTER_CLASS(className)           \
    ReflectObject * createObject##className()      \
    {                                       \
        auto obj =  new className(); \
        obj ->setClassName(#className);     \
        return obj;                         \
    }                                       \
    ClassRegister classRegister##className(#className, createObject##className);

#define REGISTER_CLASS_Field(className, name, type) \
    ClassRegister classRegister##className##name(#className,offset(className,name),#name,#type);


#define REGISTER_CLASS_METHOD(className, methodName, returnType, ...) \
    std::function<returnType(className * ,##__VA_ARGS__)> className##methodName##method = &className::methodName; \
    ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t)&(className##methodName##method));
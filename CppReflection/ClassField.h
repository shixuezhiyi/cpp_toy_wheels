//
// Created by 滕逸青 on 2022/11/23.
//

#ifndef CPPREFLECTION_CLASSFIELD_H
#define CPPREFLECTION_CLASSFIELD_H

#include <string>

using std::string;

class ClassField
{
public:
    ClassField(size_t offset = 0, const string &name = "", const string &type = "") : m_offset(offset), m_name(name),
                                                                                      m_type(type)
    {

    }

    const string &getName() const
    {
        return m_name;
    }
    const string &getType() const
    {
        return m_type;
    }

    size_t getOffset() const
    {
        return m_offset;
    }

private:
    size_t m_offset;
    string m_name;
    string m_type;
};


#endif //CPPREFLECTION_CLASSFIELD_H

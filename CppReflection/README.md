# 一个简易的 CPP 反射实现

### Usage
* A.h:
```c++
class A : public Object
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
```

* main.cpp:
```c++
    auto a = ClassFactory::getInstance().getClass("A");
    int age;
    a->set("m_age", 20);
    a->get("m_age", age);
    cout << age << endl;
    a->call("f1");
    a->call("f2", 13);
```
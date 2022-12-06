# 序列化

## 数据类型编码

### 基本数据类型

|   字段类型    |  字段长度(B)  | 底层编码格式                           |
|:---------:|:---------:|:---------------------------------|
|   bool    |     2     | Type(1) + Value(1)               |
|   char    |     2     | Type(1) + Value(1)               |
|   int32   |     5     | Type(1) + Value(4)               |
|   int64   |     9     | Type(1) + Value(8)               |
|   float   |     5     | Type(1) + Value(4)               |
|  double   |     9     | Type(1) + Value(8)               |
|  string   |   可变长度    | Type(1) + Length(5) + Value(可变)  |

### 容器编码

|   字段类型    |   字段长度(B)   | 底层编码格式                               |
|:---------:|:-----------:|:-------------------------------------|
| vector<T> |    可变长度     | Type(1)+ Length(5) + Value(T...)     |
|  list<T>  |    可变长度     | Type(1) + Length(5)+ Value(T...)     |
| map<K,V>  |    可变长度     | Type(1)+ Length(5) + Value((K,V)...) |
|  set<T>   |    可变长度     | Type(1) +Length(5)+ Value(T...)      |

### 用例
```c++
#include <iostream>
#include "DataStream.h"
#include "Serializable.h"

using namespace std;

class A : public Serializable
{
public:
    A() = default;

    ~A() = default;

    A(const int &age, const string &name, const map<string, int> &scores) : m_age(age), m_name(name), m_scores(scores)
    {}

    void show()
    {
        cout << "Name: " << m_name << "  age: " << m_age << endl;
        for (auto &it: m_scores)
            cout << it.first << ": " << it.second << endl;
    }

    SERIALIZE(m_age, m_name, m_scores);
private:
    int m_age;
    string m_name;
    map<string, int> m_scores;
};


int main()
{
    A a(15, "Tom", {{"语文", 10},
                    {"数学", 20}});
    a.show();
    DataStream ds;
    ds<<a;
    ds.save("test.out");
    DataStream ds2;
    ds2.load("test.out");
    A a2;
    ds2>>a2;
    a2.show();
}

```

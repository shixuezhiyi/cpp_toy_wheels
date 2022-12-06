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

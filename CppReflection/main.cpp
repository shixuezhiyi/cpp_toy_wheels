#include <iostream>
#include "ClassFactory.h"
#include "test/A.h"

using namespace std;


int main()
{
    auto a = ClassFactory::getInstance().getClass("A");
    int age;
    a->set("m_age", 20);
    a->get("m_age", age);
    cout << age << endl;
    a->call("f1");
    a->call("f2", 13);
}

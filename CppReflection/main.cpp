#include <iostream>
#include "ClassFactory.h"
#include "test/A.h"

using namespace std;


int main()
{
    ClassFactory::showRegister();


    auto a = ClassFactory::getInstance().createClass("A");
    int age;
    a->set("m_age", 20);
    a->get("m_age", age);
    cout << age << endl;
}

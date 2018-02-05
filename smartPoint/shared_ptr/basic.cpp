//
// Created by chenchukun on 18/2/5.
//
#include <iostream>
#include <memory>
using namespace std;

class Test
{
public:
    Test() {
        cout << "Test()" << endl;
    }

    ~Test() {
        cout << "~Test()" << endl;
    }

    void test() {
        cout << "call test()" << endl;
    }
};

int main()
{
    // make_shared 初始化一个对象,并返回该对象的智能指针
    shared_ptr<Test> p = make_shared<Test>();

    cout << "p.use_count() = " << p.use_count() << endl;
    cout << "p.unique() = " << p.unique() << endl;
    shared_ptr<Test> p2(p);

    shared_ptr<Test> p3(p2);

    cout << "p.use_count() = " << p.use_count() << endl;
    cout << "p.unique() = " << p.unique() << endl;
    cout << "p2.use_count() = " << p2.use_count() << endl;
    cout << "p2.unique() = " << p2.unique() << endl;
    cout << "p3.use_count() = " << p3.use_count() << endl;
    cout << "p3.unique() = " << p3.unique() << endl;

    p->test();

    (*p2).test();

    Test *op = p.get();
    op->test();

    return 0;
}

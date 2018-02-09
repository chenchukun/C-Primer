//
// Created by chenchukun on 18/2/8.
//
#include <iostream>
using namespace std;

/*
 * 重载operator new、operator delete
 */

void *operator new(size_t size)
{
    cout << "My global operator new" << endl;
    return malloc(size);
}

void operator delete(void *p)
{
    cout << "My global operator delete" << endl;
    free(p);
}

void *operator new[](size_t size)
{
    cout << "My global operator new[]" << endl;
    return malloc(size);
}

void operator delete[](void *p)
{
    cout << "My global operator delete[]" << endl;
    free(p);
}

class Foo
{
public:
    Foo() {
        cout << "Foo()" << endl;
    }

    ~Foo() {
        cout << "~Foo()" << endl;
    }

private:
    int x;
};

class OFoo
{
public:
    OFoo() {
        cout << "OFoo()" << endl;
    }

    ~OFoo() {
        cout << "~OFoo()" << endl;
    }

    static void* operator new(size_t size) {
        cout << "OFoo::operator new" << endl;
        return malloc(size);
    }

    static void*operator new[](size_t size) {
        cout << "OFoo::operator new[]" << endl;
        return malloc(size);
    }

    static void operator delete(void *p) {
        cout << "OFoo::operator delete" << endl;
        free(p);
    }

    static void operator delete[](void *p) {
        cout << "OFoo::operator delete[]" << endl;
        free(p);
    }

private:
    int x;
};

void global()
{
    // 调用全局的::operator new 和::operator delete
    int *p = new int;
    delete p;

    int *arr = new int[10];
    delete [] arr;

    // 调用对象内的operator new 和operator delete
    // 默认情况下会调用对应的全局版本
    Foo *pfoo = new Foo;
    delete pfoo;
}

void local()
{
    // 调用类内的operator new 和operator delete
    OFoo *p = new OFoo;
    delete p;

    p = new OFoo[10];
    delete [] p;

    // 调用全局的
    p = ::new OFoo;
    ::delete p;
}

int main()
{
    global();
    local();
    return 0;
}


//
// Created by chenchukun on 18/2/9.
//
#include <iostream>
#include <exception>
using namespace std;

/*
 * 重载placement new
 */

class Foo
{
public:
    Foo() {
        cout << "call Foo()" << endl;
    }

    ~Foo() {
        cout << "call ~Foo()" << endl;
    }

    Foo(int n) {
        throw exception();
    }

    /*
     * 可以重载各种形式的placement new,首个参数必须是size_t,之后可以包含多个其它参数
     */
    static void* operator new(size_t size, void *p) {
        cout << "call operator new(size_t size, void *p)" << endl;
        return p;
    }

    static void* operator new(size_t size, size_t extra) {
        cout << "call operator new(size_t size, size_t extra)" << endl;
        return malloc(size + extra);
    }

    /*
     * 通过placement new创建的对象,在delete时还是调用operator delete,
     * 而不是placement delete,可以重载与各版本placement new对应的placement delete
     * 这些placement delete只是在使用placement new构造对象发送异常时用于清理环境。
     */
    static void operator delete(void *p) {
        cout << "call operator delete(void *p)" << endl;
        free(p);
    }

    static void operator delete(void *p, void *p2) {
        cout << "call operator delete(void *p, void *p2)" << endl;
        free(p2);
    }

    static void operator delete(void *p, size_t extra) {
        cout << "call operator delete(void *p, size_t extra)" << endl;
        free(p);
    }

};

int main()
{
    void *vp = malloc(sizeof(Foo));
    Foo *p = new(vp)Foo();
    delete p;

    Foo *p2 = new(1)Foo();
    delete p2;

    return 0;
}


//
// Created by chenchukun on 17/11/5.
//
#include <iostream>
#include <memory>
#include <cstdlib>
using namespace std;

class Complex
{
public:
    friend ostream& operator<<(ostream &out, const Complex &com);

    Complex(int r=1, int i=1): real(r), img(i) {
        cout << "Complex(int i, int j)" << endl;
    }

    ~Complex() {
        cout << *this << " ~Complex()" << endl;
    }

private:
    int real, img;
};

ostream& operator<<(ostream &out, const Complex &com)
{
    out << "(" << com.real << ", " << com.img << ")";
    return out;
}

void newObj()
{
    Complex *cp = new(nothrow) Complex(1, 2);
    cout << *cp << endl;
    delete cp;

    // 以上语句可以展开为如下
    try {
        void *mem = ::operator new(sizeof(Complex));
        cp = reinterpret_cast<Complex*>(mem);
        // 编译器中使用 cp->Complex::Complex(1, 2)
        // 但是我们不能直接调用构造函数,可以用以下语句实现对象构造
        new(cp)Complex(1, 2);   // placement new
        cout << *cp << endl;
        // 可以直接调用析构函数
        cp->~Complex();
        ::operator delete(cp);
    } catch (bad_alloc) {
        return;
    }
}

void arrayNew()
{
    // new [] 与 delete [] 配合使用
    // new [] 如何传递构造函数参数???  没办法做到,new[] 的对象必须有默认构造函数,
    Complex *p = new Complex[3];
    for (int i=0; i<3; ++i) {
        new(p+i)Complex(i, i+3);   // 可以使用placement new
    }
    // delete [] 调用析构函数的顺序与调用构造函数的顺序相反
    delete [] p;

    // 对于当前Complex类型来说,不会造成内存泄露(类型内部没有使用动态内存),delete []指明要调用多次析构函数
    // 使用delete会造成只调用一次析构函数,分配的内存还是能够正常的释放。
    // 但是对于类内有分配动态内存的类型(即需要调用析构函数才能安全的销毁对象的类型),必须使用delete[]否则会造成内存泄露
    Complex *p2 = new Complex[3];
//    delete p2;  // g++ 会崩溃,与内存布局有关
}

void placementNew()
{
    // placement new 允许我们将对象构建在已分配的内存上。相当于不分配内存的new
    void *vp = reinterpret_cast<Complex*>(malloc(sizeof(Complex)));
    Complex *p = new(vp)Complex(1, 2);
    cout << *p << endl;
    delete p;

    // 编译器将 Complex *p = new(vp)Complex(1, 2) 转为如下语句
    // Complex *p;
    // void *men = operator new(sizeof(Complex), vp);  // 直接返回vp
    // p = reinterpret_cast<Complex*>(mem);
    // p->Complex::Complex(1, 2);
}


int main()
{
    newObj();
    arrayNew();
    placementNew();
    return 0;
}


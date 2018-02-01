//
// Created by chenchukun on 18/2/1.
//

#include <iostream>
using namespace std;

class Base
{
public:
    void base() {
        cout << "base()" << endl;
    }
};

class Drived : public Base
{
public:
    void drived() {
        cout << "drived" << endl;
    }
};
/*
 * static_cast 的作用:
 * 1、用于基础数据类型之间的转换
 * 2、用于有继承关系的类指针和引用之间的转换
 * 3、用于void指针和其他类型指针之间的转换
 */

int main()
{
    // 用于基本数据类型,编译器能够进行隐式执行的转换
    double d = static_cast<double>(10) / 3;
    cout << "d = " << d << endl;

    // 用于损失精度的类型转换,可以提醒编译器和其他看代码的人
    int x = static_cast<int>(d);
    cout << "x = " << x << endl;

    // 用于将void*转为原本类型的指针
    void *p = &x;
    int *px = static_cast<int*>(p);
    cout << *px << endl;

    // 用于有继承关系的类指针和引用之间的转换
    Drived drived;
    Drived *pd = &drived;
    // 子类指针转换为父类指针,是安全的,C++可以进行隐式转换
    Base *pb = static_cast<Base*>(pd);

    // 父类指针转为子类指针,可以进行转换,但是是不安全的,
    // 若父类指针不是指向该子类的指针,也能转换成功,但是使用时可能会出错。
    // static_cast只在编译时进行类型转换,在运行时不进行判断,安全的做法应该是使用dynamic_cast
    pd = static_cast<Drived*>(pb);

    Drived &dd = drived;
    Base &bb = static_cast<Base&>(dd);

    return 0;
}

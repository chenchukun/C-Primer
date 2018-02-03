//
// Created by chenchukun on 18/2/2.
//
#include <iostream>
using namespace std;

int main()
{
    int x = 10;
    const int *cpx = &x;
    // const_cast 可以去掉指向const的指针的const属性
    int *px = const_cast<int*>(cpx);
    *px = 20;
    cout << x << endl;

    // 将const引用转为非const引用
    const int &cx = x;
    int &xx = const_cast<int&>(cx);
    xx = 30;
    cout << x << endl;
    return 0;
}
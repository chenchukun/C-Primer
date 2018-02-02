//
// Created by chenchukun on 18/2/2.
//
#include "lib.h"
#include <iostream>
using namespace std;

// 使用extern声明函数,表示该函数可能在别的模块定义,默认情况下函数声明为extern
// 若声明为static则表明该函数在当前模块定义
extern const char *test();

int main()
{
    // 可以使用在另一个cpp文件中定义的变量和常量
    cout << MAX << endl;
    arr[0] = 10;
    cout << arr[0] << endl;
    cout << test() << endl;
    return 0;
}

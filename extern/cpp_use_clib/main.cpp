//
// Created by chenchukun on 18/1/31.
//

#ifdef __cplusplus
extern "C" {
#endif
    #include "../clibrary/calculate.h"
#ifdef __cplusplus
}
#endif
#include <iostream>
using namespace std;

/*
 * C++中使用C库在包含头文件是必须使用extern "C",
 * 用来告诉编译器,已C语言的方式连接该库
 */

int main()
{
    cout << add(3, 2) << endl;
    cout << sub(3, 2) << endl;
    return 0;
}
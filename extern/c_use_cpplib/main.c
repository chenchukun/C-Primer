//
// Created by chenchukun on 18/1/31.
//

#include "../cpplibrary/calculate.h"
#include <stdio.h>

/*
 * C 中能正常的使用C++库,若要使用C++库,必须对C++库进行简单的封装,并编译成C库
 */

int main()
{
    printf("%d\n", add(3, 2));
    printf("%d\n", sub(3, 2));
    return 0;
}
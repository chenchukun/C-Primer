//
// Created by chenchukun on 18/2/4.
//
#include <iostream>
#include <signal.h>
using namespace std;

bool q = false;

int x = 0;

void change(int signo)
{
    x = 10;
    cout << "change(): set x = 10" << endl;
}

void quit(int signo)
{
    q = true;
    cout << "quit(): set q = true" << endl;
}

int main()
{
    signal(SIGUSR1, change);
    signal(SIGUSR2, quit);
    // 这里有可能会被编译器优化为!false,导致死循环
    while (!q) {
        // 编译器有可能会将x保存到寄存器中,第二次赋值时直接使用寄存器中的值,
        // 而此时若x的值被更改,就会导致b的值不是x的最新值
        int a = x;
        int b = x;
        cout << "a = " << a << endl;
        cout << "b = " << b << endl;
    }
    return 0;
}


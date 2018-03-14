//
// Created by chenchukun on 18/3/11.
//

#include <iostream>
#include <memory>
#include <ext/pool_allocator.h>
using namespace std;

void testAllocator()
{
    // 标准库中的allocator直接使用malloc分配内存,为了能正确的释放内存,
    // 会在内存的首尾包含cookie,保存所分配的内存大小
    allocator<double> alloc;
    double *p1 = alloc.allocate(1);
    double *p2 = alloc.allocate(1);
    double *p3 = alloc.allocate(1);
    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;
    cout << "p3 = " << p3 << endl;
}

void testPoolAlloc()
{
    // __poll_alloc内部维护了一个内存池,使用其分配的内存不包含cookie
    __gnu_cxx::__pool_alloc<double> alloc;
    double *p1 = alloc.allocate(1);
    double *p2 = alloc.allocate(1);
    double *p3 = alloc.allocate(1);
    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;
    cout << "p3 = " << p3 << endl;
}

int main()
{
    testAllocator();
    cout << "------" << endl;
    testPoolAlloc();
    return 0;
}
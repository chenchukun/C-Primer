//
// Created by chenchukun on 18/3/11.
//

#include <iostream>
#include <memory>
#include <ext/pool_allocator.h>
using namespace std;

void testAllocator()
{
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
//
// Created by chenchukun on 17/11/5.
//

#include <iostream>
#include <memory>
#include <cstdlib>
using namespace std;

// C++提供的几种内存分配方式
void memoryPrimitive()
{
    // malloc 接收一个参数,表示分配的内存大小,malloc的底层是调用系统API
    int *pm = static_cast<int*>(malloc(sizeof(int)));
    free(pm);

    // new由于构建一个对象,它调用::operator new分配内存,然后调用对象的构造函数
    int *pn = new int;
    delete pn;

    // allocator是C++标准库中的内存分配器,提供标准的接口
    allocator<int> alloc;
    int *pa = alloc.allocate(1);
    alloc.construct(pa, 0);
    alloc.destroy(pa);
    alloc.deallocate(pa, 1);

    // ::operator new分配指定大小的内存空间
    void *pv = ::operator new(sizeof(int));
    int *po = static_cast<int*>(pv);
    ::operator delete(po);
}

int main()
{
    memoryPrimitive();
}
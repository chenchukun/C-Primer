//
// Created by chenchukun on 18/3/14.
//
#include <iostream>
#include <list>
#include <vector>
#include <sys/time.h>
using namespace std;

class PoolAlloc
{
public:
    typedef void (*NewHandler)();

    static void* allocate(size_t bytes);

    static void deallocate(void *p, size_t bytes);

    // DEBUG
    static size_t getAllocBytes() {
        return allocBytes_;
    }

    static void setNewHandler(NewHandler handler) {
        newHandler_ = handler;
    }

private:
    // 将内存大小调整为ALIGN的倍数
    static size_t roundUp(size_t bytes) {
        return ((bytes + ALIGN - 1) & ~(ALIGN - 1));
    }

    // 获取指定字节大小内存块在第几条空闲链表上
    static size_t freeListIndex(size_t bytes) {
        return (bytes + ALIGN -1) / ALIGN - 1;
    }

    static void* refill(size_t bytes);

    static void* chunkAlloc(size_t bytes, size_t &n);

private:
    // 内存块最小单位、 最大内存块、   每次分配的空闲链表最大长度
    enum {ALIGN = 8, MAX_BLOCK_SIZE = 128, FREE_LIST_MAX_SIZE = 20};
    // 空闲链表数组长度、    内存池最大内存数  10485760
    enum {LIST_SIZE = MAX_BLOCK_SIZE / ALIGN, MAX_MALLOC_SIZE = 100};

    struct Obj {
        struct Obj *next_;
    };

    static Obj* freeList_[LIST_SIZE];

    static size_t allocBytes_;

    static char *freeStart_;

    static char *freeEnd_;

    static NewHandler newHandler_;
};

PoolAlloc::Obj* PoolAlloc::freeList_[PoolAlloc::LIST_SIZE] =
    {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

size_t PoolAlloc::allocBytes_ = 0;

char* PoolAlloc::freeStart_ = NULL;

char* PoolAlloc::freeEnd_ = NULL;

PoolAlloc::NewHandler PoolAlloc::newHandler_ = NULL;

void* PoolAlloc::chunkAlloc(size_t bytes, size_t &n)
{
    size_t freeSize = freeEnd_ - freeStart_;

    // 内存池大小小于所需要的内存块大小,进行碎片整理
    if (freeSize > 0 && freeSize < bytes) {
        size_t index = freeListIndex(freeSize);
        Obj *obj = reinterpret_cast<Obj *>(freeStart_);
        obj->next_ = freeList_[index];
        freeList_[index] = obj;
        freeStart_ = freeEnd_ = NULL;
        return chunkAlloc(bytes, n);
    }
    // 内存池已无空间,分配更多的内存
    if (freeSize == 0) {
        size_t mallocSize = bytes * n * 2 + roundUp(allocBytes_>>4);
        freeStart_ = static_cast<char*>(::operator new(mallocSize));
        // 内存分配失败, 尝试从更大的内存块中取出一块作为内存池使用
        if (freeStart_ == NULL) {
            size_t index = freeListIndex(bytes);
            for (size_t i=index+1; i<LIST_SIZE; ++i) {
                Obj *obj = freeList_[i];
                if (obj != NULL) {
                    freeStart_ = reinterpret_cast<char*>(obj);
                    freeEnd_ = freeStart_ + (i + 1) * ALIGN;
                    freeList_[i] = obj->next_;
                    return chunkAlloc(bytes, n);
                }
            }
            // 分配内存失败,且没有更大的内存块可用
            if (newHandler_ != NULL) {
                newHandler_();
                return chunkAlloc(bytes, n);
            }
            else {
                throw bad_alloc();
            }
        }
        else {
            allocBytes_ += mallocSize;
            freeEnd_ = freeStart_ + mallocSize;
            void *pret = static_cast<void*>(freeStart_);
            freeStart_ += bytes * n;
            return pret;
        }
    }
    else {
        // 内存池容量大于等于内存块大小
        void *pret = static_cast<void*>(freeStart_);
        size_t maxBlocks = freeSize / bytes;
        n = maxBlocks > n? n: maxBlocks;
        freeStart_ += bytes * n;
        return pret;
    }
}

void* PoolAlloc::refill(size_t bytes)
{
    size_t n = FREE_LIST_MAX_SIZE;
    char *chunk = static_cast<char*>(chunkAlloc(bytes, n));
    if (n == 1) {
        return static_cast<void*>(chunk);
    }
    size_t index = freeListIndex(bytes);
    for (size_t i=1; i < n; ++i) {
        Obj *obj = reinterpret_cast<Obj*>(chunk + i * bytes);
        if (i == n-1) {
            obj->next_ = NULL;
            break;
        }
        Obj *next = reinterpret_cast<Obj*>(chunk + (i + 1) * bytes);
        obj->next_ = next;
    }
    freeList_[index] = reinterpret_cast<Obj*>(chunk + bytes);
    return static_cast<void*>(chunk);
}

void* PoolAlloc::allocate(size_t bytes)
{
    if (bytes > MAX_BLOCK_SIZE) {
        return ::operator new(bytes);
    }
    size_t alignBytes = roundUp(bytes);
    size_t index = freeListIndex(alignBytes);
    // 空闲链表为空
    if (freeList_[index] == NULL) {
        return refill(alignBytes);
    }
    Obj *head = freeList_[index];
    freeList_[index] = head->next_;
    return static_cast<void*>(head);
}

void PoolAlloc::deallocate(void *p, size_t bytes)
{
    if (p == NULL) {
        return;
    }
    if (bytes > MAX_BLOCK_SIZE) {
        ::operator delete(p);
    }
    size_t index = freeListIndex(bytes);
    Obj *head = static_cast<Obj*>(p);
    head->next_ = freeList_[index];
    freeList_[index] = head;
}

template<typename T>
class Alloc
{
public:
    static T* allocate(size_t n);

    static void deallocate(T *p, size_t n);

    template<typename... AT>
    static void construct(T *p, AT... args);

    static void destroy(T *p);

    static void setNewHandler(PoolAlloc::NewHandler handler) {
        PoolAlloc::setNewHandler(handler);
    }

    typedef T value_type;
};

template<typename T>
T* Alloc<T>::allocate(size_t n)
{
    void *p = PoolAlloc::allocate(sizeof(T) * n);
    return static_cast<T*>(p);
}

template<typename T>
void Alloc<T>::deallocate(T *p, size_t n)
{
    PoolAlloc::deallocate(static_cast<void*>(p), sizeof(T) * n);
}

template<typename T>
template<typename... AT>
void Alloc<T>::construct(T *p, AT... args)
{
    new(p)T(args...);
}

template<typename T>
void Alloc<T>::destroy(T *p)
{
    p->~T();
}

class Foo
{
public:
    Foo() {
        x_ = y_ = 0;
        b_ = false;
        cout << "call Foo()" << endl;
    }

    Foo(int x) {
        x_ = y_ = x;
        b_ = false;
        cout << "call Foo(int)" << endl;
    }

    Foo(const Foo &f) {
        cout << "call Foo(const Foo&)" << endl;
    }

    ~Foo() {
        cout << "call ~Foo()" << endl;
    }

private:
    int x_;
    int y_;
    bool b_;
};


void testAllocInt()
{
    cout << "sizeof(int) = " << sizeof(int) << endl;
    Alloc<int> alloc;
    int *p1 = alloc.allocate(1);
    alloc.construct(p1, 10);
    int *p2 = alloc.allocate(1);
    alloc.construct(p2, 20);
    cout << "p1 = " << *p1 << " " << p1 << endl;
    cout << "p2 = " << *p2 << " " << p2 << endl;
    alloc.destroy(p1);
    alloc.destroy(p2);
    alloc.deallocate(p1, 1);
    alloc.deallocate(p2, 1);
    cout << "allocBytes = " << PoolAlloc::getAllocBytes() << endl;
}

void testAllocFoo()
{
    cout << "sizeof(Foo) = " << sizeof(Foo) << endl;
    Alloc<Foo> alloc;
    Foo *f1 = alloc.allocate(1);
    Foo *f2 = alloc.allocate(1);

    cout << "f1 = " << f1 << endl;
    cout << "f2 = " << f2 << endl;

    alloc.construct(f1);
    alloc.destroy(f1);
    alloc.deallocate(f1, 1);

    alloc.construct(f2, 10);
    alloc.destroy(f2);
    alloc.deallocate(f2, 1);
    cout << "allocBytes = " << PoolAlloc::getAllocBytes() << endl;
}

void newHandler()
{
    cerr << "out of memory" << endl;
    abort();
}

size_t number = 0;

size_t total = 0;

const size_t TEST_COUNT = 100000000;

void* operator new(size_t n)
{
    ++number;
    total += n;
    return malloc(n);
}

template<typename T>
using PoolList = list<T, Alloc<T>>;

void testAlloc()
{
    number = total = 0;
    timeval start, end;
    PoolList<double> l;
    gettimeofday(&start, NULL);
    for (int i=0; i<TEST_COUNT; ++i) {
        l.push_back(i);
    }
    gettimeofday(&end, NULL);
    cout << "==========testAlloc==========" << endl;
    cout << "number of malloc: " << number << endl;
    cout << "total of memory: " << total << endl;
    uint32_t use = (end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec);
    cout << "time: " << use << " usec" << endl;
}

void testAllocator()
{
    number = total = 0;
    timeval start, end;
    list<double> l;
    gettimeofday(&start, NULL);
    for (int i=0; i<TEST_COUNT; ++i) {
        l.push_back(i);
    }
    gettimeofday(&end, NULL);
    cout << "==========testAllocator==========" << endl;
    cout << "number of malloc: " << number << endl;
    cout << "total of memory: " << total << endl;
    uint32_t use = (end.tv_sec*1000000 + end.tv_usec) - (start.tv_sec*1000000 + start.tv_usec);
    cout << "time: " << use << " usec" << endl;
}

int main()
{
    PoolAlloc::setNewHandler(newHandler);
//    testAllocInt();
//    testAllocFoo();
    testAlloc();
    testAllocator();
    return 0;
}

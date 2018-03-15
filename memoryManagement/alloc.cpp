//
// Created by chenchukun on 18/3/14.
//
#include <iostream>
using namespace std;

class PoolAlloc
{
public:
    static void* allocate(size_t bytes);

    static void deallocate(void *p, size_t bytes);

    // DEBUG
    static size_t getAllocBytes() {
        return allocBytes_;
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
    // 空闲链表数组长度、    内存池最大内存数
    enum {LIST_SIZE = MAX_BLOCK_SIZE / ALIGN, MAX_MALLOC_SIZE = 10485760};

    struct Obj {
        struct Obj *next_;
    };

    static Obj* freeList_[LIST_SIZE];

    static size_t allocBytes_;

    static char *freeStart_;

    static char *freeEnd_;
};

PoolAlloc::Obj* PoolAlloc::freeList_[PoolAlloc::LIST_SIZE] = {NULL};

size_t PoolAlloc::allocBytes_ = 0;

char* PoolAlloc::freeStart_ = NULL;

char* PoolAlloc::freeEnd_ = NULL;

void* PoolAlloc::chunkAlloc(size_t bytes, size_t &n)
{
    size_t freeSize = freeEnd_ - freeStart_;
    // 碎片整理
    if (freeSize > 0 && freeSize < bytes) {
        size_t index = freeListIndex(freeSize);
        Obj *obj = reinterpret_cast<Obj*>(freeStart_);
        obj->next_ = freeList_[index];
        freeList_[index] = obj;
        freeStart_ = freeEnd_ = NULL;
    }
    if (freeStart_ == NULL && freeEnd_ == NULL) {
        freeStart_ = static_cast<char*>(malloc(bytes * n * 2));
        allocBytes_ += bytes * n *2;
        freeEnd_ = freeStart_ + bytes * n * 2;
        freeStart_ += bytes * n;
        return freeStart_;
    }
    n = freeSize / bytes;
    freeStart_ += bytes * n;
    return freeStart_;
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
    static T* allocate();

    static void deallocate(T *p);

    template<typename... AT>
    static void construct(T *p, AT... args);

    static void destroy(T *p);
};

template<typename T>
T* Alloc<T>::allocate()
{
    void *p = PoolAlloc::allocate(sizeof(T));
    return static_cast<T*>(p);
}

template<typename T>
void Alloc<T>::deallocate(T *p)
{
    PoolAlloc::deallocate(static_cast<void*>(p), sizeof(T));
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
    int *p1 = alloc.allocate();
    alloc.construct(p1, 10);
    int *p2 = alloc.allocate();
    alloc.construct(p2, 20);
    cout << "p1 = " << *p1 << " " << p1 << endl;
    cout << "p2 = " << *p2 << " " << p2 << endl;
    alloc.destroy(p1);
    alloc.destroy(p2);
    alloc.deallocate(p1);
    alloc.deallocate(p2);
    cout << "allocBytes = " << PoolAlloc::getAllocBytes() << endl;
}

void testAllocFoo()
{
    cout << "sizeof(Foo) = " << sizeof(Foo) << endl;
    Alloc<Foo> alloc;
    Foo *f1 = alloc.allocate();
    Foo *f2 = alloc.allocate();

    cout << "f1 = " << f1 << endl;
    cout << "f2 = " << f2 << endl;

    alloc.construct(f1);
    alloc.destroy(f1);
    alloc.deallocate(f1);

    alloc.construct(f2, 10);
    alloc.destroy(f2);
    alloc.deallocate(f2);
    cout << "allocBytes = " << PoolAlloc::getAllocBytes() << endl;
}

int main()
{
    testAllocInt();
    testAllocFoo();
    return 0;
}

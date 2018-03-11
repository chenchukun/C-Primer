//
// Created by chenchukun on 18/3/10.
//
#include <iostream>
using namespace std;

template<typename T>
class Allocator
{
public:
    Allocator(): store_(NULL) {}

    T* allocate();

    void deallocate(T *p);

    template<typename... AT>
    void construct(T *p, AT... args);

    void destroy(T *p);

private:

    enum {BLOCK_SIZE=3};

    union TRep {
        T t;
        TRep *next;
    };

    TRep *store_;
};

template<typename T>
T* Allocator<T>::allocate()
{
    if (store_ == NULL) {
        void *p = malloc(sizeof(TRep) * BLOCK_SIZE);
        store_ = static_cast<TRep*>(p);
        for (int i=0; i<BLOCK_SIZE-1; ++i) {
            store_[i].next = &store_[i+1];
        }
        store_[BLOCK_SIZE-1].next = NULL;
    }
    T *ret = reinterpret_cast<T*>(store_);
    store_ = store_->next;
    return ret;
}

template<typename T>
void Allocator<T>::deallocate(T *p)
{
    TRep *t = reinterpret_cast<TRep*>(p);
    t->next = store_;
    store_ = t;
}

template<typename T>
template<typename... AT>
void Allocator<T>::construct(T *p, AT... args)
{
    new(p)T(args...);
}

template<typename T>
void Allocator<T>::destroy(T *p)
{
    p->~T();
}

class Foo
{
public:
    Foo() {
        cout << "call Foo()" << endl;
    }

    Foo(int x) {
        cout << "call Foo(int)" << endl;
    }

    Foo(const Foo &f) {
        cout << "call Foo(const Foo&)" << endl;
    }

    ~Foo() {
        cout << "call ~Foo()" << endl;
    }
};

int main()
{
    Allocator<int> allocator;
    int *a = allocator.allocate();
    allocator.construct(a, 10);
    cout << *a << endl;
    allocator.destroy(a);
    allocator.deallocate(a);

    Allocator<Foo> fallocator;
    Foo *f = fallocator.allocate();

    fallocator.construct(f);
    fallocator.destroy(f);

    fallocator.construct(f, 10);
    fallocator.destroy(f);
    fallocator.deallocate(f);
    return 0;
}

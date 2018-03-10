//
// Created by chenchukun on 18/3/10.
//
#include <iostream>

template<typename T>
class Allocator
{
public:
    Allocator(): store_(NULL) {}

    T* allocate(size_t n);

    void deallocate(T *p, size_t n);

    void construct(T *p, const T &t);

    void destroy(T *p);

private:

    enum {BLOCK_SIZE=3};

    T *store_;
};

template<typename T>
T* Allocator<T>::allocate(size_t n)
{

    return NULL;
}

template<typename T>
void Allocator<T>::deallocate(T *p, size_t n)
{

}

template<typename T>
void Allocator<T>::construct(T *p, const T &t)
{

}

template<typename T>
void Allocator<T>::destroy(T *p)
{

}

int main()
{
    return 0;
}

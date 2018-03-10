//
// Created by chenchukun on 18/3/10.
//
#include <iostream>
using namespace std;

class Complex
{
public:
    friend ostream& operator<<(ostream &out, const Complex &com);

    void setReal(int real) {
        rep_.real_ = real;
    }

    void setImg(int img) {
        rep_.img_ = img;
    }

    int getReal() const {
        return rep_.real_;
    }

    int getImg() const {
        return rep_.img_;
    }

    static void* operator new(size_t size) {
        if (size != sizeof(Complex)) {
            return ::operator new(size);
        }

        if (freeStore == NULL) {
            void *p = ::operator new(sizeof(Complex) * BLOCK_SIZE);
            freeStore = static_cast<Complex*>(p);
            for (int i=0; i<BLOCK_SIZE-1; ++i) {
                freeStore[i].next_ = &freeStore[i+1];
            }
            freeStore[BLOCK_SIZE-1].next_ = NULL;
            freeStore = freeStore->next_;
            return p;
        }
        else {
            Complex *p = freeStore;
            freeStore = freeStore->next_;
            return p;
        }
    }

    static void operator delete(void *p) {
        Complex *cp = static_cast<Complex*>(p);
        cp->next_ = freeStore;
        freeStore = cp;
    }

private:
    struct ComplexRep {
        int real_;
        int img_;
    };

    union {
        ComplexRep rep_;
        Complex *next_;
    };

private:
    static Complex *freeStore;

    enum {BLOCK_SIZE=2};
};

Complex* Complex::freeStore = NULL;

ostream& operator<<(ostream &out, const Complex &com)
{
    out << "complex(" << com.getReal() << ", " << com.getImg() << ")";
    return out;
}

int main()
{
    Complex *p = new Complex;
    cout << "p = " <<  p << endl;
    delete p;

    p = new Complex;
    cout << "p = " << p << endl;
    delete p;

    Complex *p1, *p2, *p3;
    p1 = new Complex;
    p2 = new Complex;
    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;
    delete p1;
    delete p2;

    p1 = new Complex;
    p2 = new Complex;
    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;
    delete p1;
    delete p2;


    p1 = new Complex;
    p2 = new Complex;
    p3 = new Complex;
    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;
    cout << "p3 = " << p3 << endl;
    delete p1;
    delete p2;
    delete p3;

    return 0;
}
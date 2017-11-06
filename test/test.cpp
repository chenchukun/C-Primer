#include <iostream>
using namespace std;

template <typename T>
class Complex
{
public:
    template<typename S>
    friend ostream& operator<<(ostream& out, const Complex<S> &com);

    Complex(T i, T j)
        : re(i), im(j) {}

private:
    T re, im;
};


template <typename T>
ostream& operator<<(ostream& out, const Complex<T> &com)
{
    out << "(" << com.re << ", " << com.im << ")";
    return out;
}



class String
{
public:
    friend ostream& operator<<(ostream &out, const String &str);

    String(const char *str=NULL);

    String(const String &str);

    String& operator=(const String &str);

    ~String();

    const char* c_str() const {
        return data;
    }

    size_t size() const {
        return strlen(data);
    }

private:
    char *data;
};

ostream& operator<<(ostream &out, const String &str)
{
    out << str.c_str();
    return out;
}

String::String(const char *str)
{
    if (str == NULL) {
        data = new char[1];
        data[0] = 0;
    }
    else {
        data = new char[strlen(str) + 1];
        strcpy(data, str);
    }
}

String::String(const String &str)
{
    data = new char[strlen(str.c_str()) + 1];
    strcpy(data, str.c_str());
}

String& String::operator=(const String &str)
{
    if (&str == this) {
        return *this;
    }
    const char *ptmp = data;
    data = new char[strlen(str.c_str()) + 1];
    strcpy(data, str.c_str());
    delete(ptmp);
    return *this;
}

String::~String()
{
    delete [] data;
}

template <typename T>
void quickSort(T *begin, T *end)
{
    if (end-begin > 1) {
        T val = *begin;
        T *low = begin;
        T *hight = end - 1;
        while (low < hight) {
            while ((low < hight) && *hight >= val) {
                --hight;
            }
            *low = *hight;
            while ((low < hight) && *low <= val) {
                ++low;
            }
            *hight = *low;
        }
        *low = val;
        quickSort(begin, low);
        quickSort(low+1, end);
    }
}

int main() {
    Complex<int> com(1, 2);
    cout << com << endl;

    String str;
    cout << str << endl;
    str = "hello world";
    cout << str << endl;
    String str1 = str;
    cout << str1 << endl;
    String str2("hello");
    cout << str2 << endl;
    int array[] = {2, 43, 80, 43, 89, 12, 89, 99, 10, 43};
    for_each(begin(array), end(array), [](int val){cout << val << "\t";});
    cout << endl;
    quickSort(begin(array), end(array));
    for_each(begin(array), end(array), [](int val){cout << val << "\t";});
    cout << endl;
    return 0;
}
//
// Created by chenchukun on 18/3/10.
//

#include <iostream>
using namespace std;

class Foo
{
public:
    struct {
        int a;
        int b;
    };
    union {
        int x;
        float y;
    };
};

int main()
{
    Foo foo;
    foo.a = 10;
    cout << "foo.a = " << foo.a << endl;
    foo.b = 20;
    cout << "foo.b = " << foo.b << endl;
    foo.x = 10;
    cout << "foo.x = " << foo.x << endl;
    cout << "foo.y = " << foo.y << endl;
    foo.y = 6.18;
    cout << "foo.x = " << foo.x << endl;
    cout << "foo.y = " << foo.y << endl;

    cout << sizeof(Foo) << endl;
    return 0;
}
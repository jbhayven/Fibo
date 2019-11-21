#include <cassert>
#include <climits>
#include <iostream>
#include "fibo.h"

int main() {
    Fibo f;
    
    assert(f == Zero());
    assert(Fibo(f) == Zero());
    assert(Zero() < One());
    assert(Fibo("11") == Fibo("100"));
    assert((Fibo("1001") + Fibo("10")) == Fibo("1011"));
    assert((Fibo("1001") & Fibo("1100")) == Zero()); // 1100 == 10000
    assert((Fibo("1100") | Fibo("11")) == Fibo("10100")); // 1100 == 10000, 11 == 100
    assert((Fibo("1001") ^ Fibo("1010")) == Fibo("11"));
    assert((Fibo("101") << 3) == Fibo("101000"));

    f = One();
    f <<= 3;
    assert(f == Fibo("1000"));

    f = One();
    assert(f + Fibo("1") == Fibo("10"));
    assert(f == One());

    Fibo f1("101");
    Fibo f2 = Fibo("101");
    assert(f1 == f2);

    assert(Fibo("11").length() == 3); // 11 == 100

    srand(time(NULL));
    using namespace std;
    
    string str1 = "10101000";
    string str2 =  "1001001";
    
    cout << str1 << endl;
    cout << str2 << endl;
    cout << Fibo(str1) << endl;
    cout << Fibo(str2) << endl;
    cout << (Fibo(str1) & Fibo(str2)) << endl;
    cout << (Fibo(str1) ^ Fibo(str2)) << endl;
    cout << (Fibo(str1) | Fibo(str2)) << endl;
}

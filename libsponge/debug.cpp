#include <iostream>
using namespace std;

#include "wrapping_integers.hh"

void check_wrap(){
    uint64_t base = static_cast<uint64_t>(1) << 32;
    uint64_t n = 15 + base;
    WrappingInt32 isn(21);
    WrappingInt32 b = wrap(n, isn);
    cout << b << endl;
}

void check_unwrap(){
    uint64_t base = static_cast<uint64_t>(1) << 32;
    WrappingInt32 n(UINT32_MAX);
    WrappingInt32 isn(0);
    uint64_t checkpoint = 0;
    uint64_t b = unwrap(n, isn, checkpoint);
    cout << b << endl;
}
int main(){
    check_unwrap();
    return 0;
}

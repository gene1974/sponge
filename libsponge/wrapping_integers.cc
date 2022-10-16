#include "wrapping_integers.hh"
#include <iostream>

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    DUMMY_CODE(n, isn);
    uint32_t res = uint32_t(n % (static_cast<uint64_t>(1) << 32)) + isn.raw_value();
    return WrappingInt32(res);
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    DUMMY_CODE(n, isn, checkpoint);
    uint64_t base = static_cast<uint64_t> (1) << 32;
    uint64_t res = checkpoint - checkpoint % base;
    res = res + n.raw_value() - isn.raw_value(); // WrappingInt32的减法返回值为int32，会溢出
    if(n.raw_value() < isn.raw_value()) res += base; // n不小于isn，如果n小于isn说明循环
    
    // 寻找最近的checkpoint
    if(res <= checkpoint){
        if(res <= UINT64_MAX - base && res + base - checkpoint < checkpoint - res){ // 防止溢出
            res = res + base;
        }
    }
    else{ // res > checkpoint
        if(res >= base && checkpoint + base - res < res - checkpoint){
            res = res - base;
        }
    }
    return res;
}

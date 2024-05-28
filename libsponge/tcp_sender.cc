#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) {}

uint64_t TCPSender::bytes_in_flight() const {
    return {};
}

void TCPSender::fill_window() {
    while(true && !stream_in().buffer_empty()){
        Buffer payload = stream_in().read(TCPConfig::MAX_PAYLOAD_SIZE);
        TCPSegment seg;
        seg.payload() = payload;
        seg.header().seqno = wrap(_next_seqno, _isn);
        _segments_out.push(seg);
        _outstand[_next_seqno] = seg; // track outstanding
        _next_seqno += 1;
    }
    // TODO: window size
    // while(true && !_segments_out.empty()){
    //     TCPSegment seg = _segments_out.front();
    //     string str = seg.serialize().concatenate();
    //     _stream.write(str);
    // }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    DUMMY_CODE(ackno, window_size);
    _window_size = window_size;
    // todo: new window_size?
    // TODO: clear outstand
}

void TCPSender::check_ack(){
    if (_cur_time - _timer[unwrap(_ackno, _isn, 0)] >= _retrans_timeout){
        // retrans
        _retrans_num += 1;
    }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    DUMMY_CODE(ms_since_last_tick);
    _cur_time += ms_since_last_tick;
    check_ack();
}

unsigned int TCPSender::consecutive_retransmissions() const { return {}; }

void TCPSender::send_empty_segment() {
    TCPSegment seg;
    seg.header().ackno = WrappingInt32(0);
    _segments_out.push(seg);
}

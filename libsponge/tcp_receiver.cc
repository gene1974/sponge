#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    DUMMY_CODE(seg);
    if(!_syn && seg.header().syn){
        _syn = true;
        _isn = seg.header().seqno;
        _ack = seg.header().seqno + 1; // SYN has one seqno
    }
    if(seg.header().fin){ // record FIN seqno(for ACK)
        _end_idx = seg.header().seqno + seg.length_in_sequence_space();
    }

    string data = seg.payload().copy();
    uint64_t checkpoint = _reassembler.stream_out().bytes_written(); // TODO: checkpoint
    uint64_t index = unwrap(seg.header().seqno, _isn, checkpoint);
    bool eof = seg.header().fin;
    if(seg.header().syn){ // 有SYN的数据seqno从1开始
        _reassembler.push_substring(data, index, eof);
    }
    else{
        _reassembler.push_substring(data, index - 1, eof);
    }
    // adjust ack
    _ack = wrap(_reassembler.stream_out().bytes_written(), _isn) + 1; // ACK是下一个序号+1
    if(_ack == _end_idx - 1){
        _ack = _ack + 1; // FIN occupies one seqno
    }
}

std::optional<WrappingInt32> TCPReceiver::ackno() const {
    // cout << _capacity << ' ' << _reassembler.stream_out().bytes_read() << ' ' << _reassembler.stream_out().bytes_written() << ' ' << _reassembler.unassembled_bytes() << endl;
    if(_syn){
        return _ack;
    }
    return std::nullopt;
}

size_t TCPReceiver::window_size() const {
    // return _capacity - _reassembler.unassembled_bytes() - _reassembler.stream_out().bytes_written();
    // cout << _capacity << ' ' << _reassembler.stream_out().bytes_read() << ' ' << _reassembler.stream_out().bytes_written() << ' ' << _reassembler.unassembled_bytes() << endl;
    return _capacity - (_reassembler.stream_out().bytes_written() - _reassembler.stream_out().bytes_read());
}

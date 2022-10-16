#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity)\
    , _cur_index(0), _end_index(0xffffffff), _buffer() {}


//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
    if(eof){
        _end_index = index + data.length();
    }
    string new_data = data;
    size_t new_index = index;
    if(index + data.size() <= _cur_index){ // end等于cur_index也无法发送
        if(eof) _output.end_input(); // 如果结束了则关闭流
        return;
    }
    if(index < _cur_index){ // 已经发送的部分删掉
        new_data = data.substr(_cur_index - index);
        new_index = _cur_index;
    }
    // 先存入buffer，模拟reassemble的过程，避免发送超出窗口范围的数据
    for(size_t i = 0; i < new_data.length() && new_index + i < _capacity + _output.bytes_read(); i++){
        _buffer[new_index + i] = new_data[i];
    }
    if(index <= _cur_index){ // 可以发送数据
        new_data = "";
        while(_buffer.count(_cur_index) && _cur_index < _end_index){
            new_data += _buffer[_cur_index];
            _buffer.erase(_cur_index);
            _cur_index += 1;
        }
        _output.write(new_data);
    }
    if(_cur_index >= _end_index){
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const {
    return _buffer.size();
}

bool StreamReassembler::empty() const {
    return _buffer.empty();
}

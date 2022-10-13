#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity)\
    , _cur_index(0), _end_index(capacity), _unassembled_bytes(0), _buffer(_capacity) {}

// 发送Data中的数据
void StreamReassembler::_send_str(const string data){
    if(_cur_index + data.length() > _end_index){
        _output.write(data.substr(0, _end_index - _cur_index));
    }
    _output.write(data);
    _cur_index += data.length();
}

// 检查buffer中是否有可以发送的数据
void StreamReassembler::_check_overlap(){
    for(size_t i = 0; i < _buffer.size(); i++){
        if(_buffer[i].second < _cur_index){ // 旧数据包被覆盖
            if(_buffer[i].second + _buffer[i].first.length() < _cur_index){
                _unassembled_bytes -= _buffer[i].first.length();
                _buffer.erase(_buffer.begin() + i);
            }
            else{ // 没完全覆盖
                _send_str(_buffer[i].first.substr(_cur_index - _buffer[i].second));
                _unassembled_bytes -= _buffer[i].first.length();
                _buffer.erase(_buffer.begin() + i);
                i = -1; // 发送新数据之后重新检查
            }
        }
        else if(_buffer[i].second == _cur_index){
            _send_str(_buffer[i].first);
            _unassembled_bytes -= _buffer[i].first.length();
            _buffer.erase(_buffer.begin() + i);
            i = -1; // 发送新数据之后重新检查
        }
    }
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
    // TODO: 需要处理capacity
    if(eof){
        _end_index = index + data.length();
    }
    string new_data = data;
    size_t begin_pos = index, end_pos = index + data.length(); // data流结束位置
    if(index <= _cur_index){ // 可以发送数据
        if(end_pos < _cur_index){ // 发来了已经接收到的数据
            return;
        }
        _send_str(data.substr(_cur_index - index));
        _check_overlap();
        return;
    }
    // 不缓存重复数据
    for(size_t i = 0; i < _buffer.size(); i++){ // 对于缓冲区中的每一个数据，确保没有overlap
        size_t buf_begin = _buffer[i].second, buf_end = _buffer[i].second + _buffer[i].first.length();
        if(buf_begin < begin_pos && buf_end > end_pos){ // 旧数据范围覆盖新数据
            return; // 不添加新数据
        }
        else if(begin_pos < buf_begin && end_pos > buf_end){ // 新数据范围覆盖旧数据
            _unassembled_bytes -= _buffer[i].first.length();
            _buffer.erase(_buffer.begin() + i); // 删除旧数据
        }
        else if (begin_pos < buf_begin && end_pos > buf_begin){ // 新数据末尾与旧数据头部重合
            new_data = new_data.substr(0, buf_begin - begin_pos);
            end_pos = buf_begin;
        }
        else if (begin_pos < buf_end && end_pos > buf_end){ // 新数据头部与旧数据末尾重合
            new_data = new_data.substr(buf_end - begin_pos);
            begin_pos = buf_end;
        }
        
    }
    if(_cur_index + _unassembled_bytes + new_data.length() > _capacity){
        new_data = new_data.substr(0, _capacity - _unassembled_bytes - _cur_index);
    }
    _buffer.push_back(make_pair(new_data, begin_pos));
    _unassembled_bytes += new_data.length();
}

size_t StreamReassembler::unassembled_bytes() const {
    return _unassembled_bytes;
}

bool StreamReassembler::empty() const {
    return _buffer.empty();
}

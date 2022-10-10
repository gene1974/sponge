#include "byte_stream.hh"
#include <cstring>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity):
    _capacity(capacity), _buffer(""), 
    _written(0), _read(0),
    _input_end(false) {}

size_t ByteStream::write(const string &data) {
    size_t len = min(data.size(), _capacity - _buffer.length());
    _buffer = _buffer + data.substr(0, len);
    _written += len;
    _input_end = false;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string res = _buffer.substr(0, len);
    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    size_t size = min(len, _buffer.length());
    _read = _read + size; // record read bytes
    _buffer = _buffer.substr(size);
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string res = _buffer.substr(0, len);
    _buffer = _buffer.substr(len);
    _read += len;
    return res;
}

void ByteStream::end_input() {
    _input_end = true;
}

bool ByteStream::input_ended() const {
    return _input_end;
}

size_t ByteStream::buffer_size() const { 
    return _buffer.length();
}

bool ByteStream::buffer_empty() const {
    return _buffer.empty();
}

bool ByteStream::eof() const { // _input_end && _buffer.empty() means end of file
    return _input_end && _buffer.empty();
}

size_t ByteStream::bytes_written() const {
    return _written;
}

size_t ByteStream::bytes_read() const {
    return _read;
}

size_t ByteStream::remaining_capacity() const {
    return _capacity - _buffer.size();
}

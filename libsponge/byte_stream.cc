#include "byte_stream.hh"
#include <algorithm>
#include <cassert>
// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity): inner_buf_(capacity + 1,'\0'),
buffer_len_(capacity + 1)
{ 
}

size_t ByteStream::write(const string &data) {
    size_t len = std::min(data.size(),remaining_capacity());
    if(end_ + len <= buffer_len_)
    {
        std::copy(data.begin(), data.begin() + len, inner_buf_.begin() + end_);
    }
    else
    {
        std::copy(data.begin(),data.begin() + (buffer_len_ - end_),inner_buf_.begin() + end_);
        std::copy(data.begin() + (buffer_len_ - end_),data.begin() + len,inner_buf_.begin());
    }
    end_ = (end_ + len) % buffer_len_ ;
    written_sum_ += len;
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    string ret_data;
    assert(len <= buffer_size());
    if(start_ + len + 1 <= buffer_len_)
    {
        ret_data.append(inner_buf_.begin() + start_,inner_buf_.begin() + start_ + len);
    }
    else
    {
        ret_data.append(inner_buf_.begin() + start_,inner_buf_.end());
        ret_data.append(inner_buf_.begin(),inner_buf_.begin() + (start_ + len  - buffer_len_));
    }
    return ret_data;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    assert(len <= buffer_size());
    start_ = (start_ + len) % buffer_len_;
    read_sum_ += len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string ret_data = peek_output(len);
    pop_output(len);
    return ret_data;
}

void ByteStream::end_input() {input_ended_ = true;}

bool ByteStream::input_ended() const { return input_ended_; }

size_t ByteStream::buffer_size() const { return ((end_ + buffer_len_) - start_) % buffer_len_; }

bool ByteStream::buffer_empty() const { return  start_ == end_; }

bool ByteStream::eof() const { return input_ended_ && buffer_empty(); }

size_t ByteStream::bytes_written() const { return written_sum_; }

size_t ByteStream::bytes_read() const { return read_sum_; }

size_t ByteStream::remaining_capacity() const { return buffer_len_ - 1 - buffer_size(); }

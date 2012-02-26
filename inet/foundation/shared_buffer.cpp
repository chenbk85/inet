#include "shared_buffer.h"

namespace inet {

shared_buffer::shared_buffer()
	: begin_(0)
	, end_(0)
	, capacity_(0)
{
}

shared_buffer::shared_buffer(uint32 size)
	: buffer_(new uint8[size])
	, begin_(0)
	, end_(size)
	, capacity_(end_)
{
}

shared_buffer::shared_buffer(const_buffer& buffer)
	: begin_(0)
	, end_(buffer_size(buffer))
	, capacity_(end_)
{
	buffer_.reset(new uint8[capacity_]);
	memcpy(buffer_.get(), buffer_cast<const uint8*>(buffer), capacity_);
}

shared_buffer::shared_buffer(mutable_buffer& buffer)
	: begin_(0)
	, end_(buffer_size(buffer))
	, capacity_(end_)
{
	buffer_.reset(new uint8[capacity_]);
	memcpy(buffer_.get(), buffer_cast<uint8*>(buffer), capacity_);
}

shared_buffer::~shared_buffer()
{
}

shared_buffer& shared_buffer::restore()
{
	begin_ = 0;
	return *this;
}

shared_buffer& shared_buffer::resize(uint32 size)
{
	begin_ = 0;
	if(capacity_ < size) {
		uint8 *buffer = new uint8[size];
		memcpy(buffer, buffer_.get(), capacity_);
		buffer_.reset(buffer);

		capacity_ = size;
		end_ = size;
	} else {
		end_ = size;
	}
	return *this;
}

shared_buffer& shared_buffer::remove(uint32 size)
{
	if(this->size() < size) {
		begin_ = end_;
	} else {
		begin_ += size;
	}
	return *this;
}

uint8* shared_buffer::bytes()
{
	return buffer_.get() + begin_;
}

const uint8* shared_buffer::bytes() const
{
	return buffer_.get() + begin_;
}

uint32 shared_buffer::size() const
{
	return end_ - begin_;
}

bool shared_buffer::empty() const
{
	return size() == 0;
}

uint32 shared_buffer::capacity() const
{
	return capacity_;
}

}
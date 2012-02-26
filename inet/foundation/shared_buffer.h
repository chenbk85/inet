#pragma once

#include <boost/shared_array.hpp>

namespace inet {

class shared_buffer
{
public:
	shared_buffer();
	explicit shared_buffer(uint32 size);
	template <typename POD_TYPE, uint32 N>
	explicit shared_buffer(POD_TYPE (&buffer)[N]);
	template <typename POD_TYPE>
	shared_buffer(POD_TYPE* buffer, uint32 size);
	explicit shared_buffer(const_buffer& buffer);
	explicit shared_buffer(mutable_buffer& buffer);
	virtual ~shared_buffer();

	shared_buffer& restore();
	shared_buffer& resize(uint32 size);
	shared_buffer& remove(uint32 size);
	uint8* bytes();
	const uint8* bytes() const;
	uint32 size() const;
	bool empty() const;
	uint32 capacity() const;

protected:
	boost::shared_array<uint8> buffer_;
	uint32 begin_, end_, capacity_;
};


template <typename POD_TYPE, uint32 N>
shared_buffer::shared_buffer(POD_TYPE (&buffer)[N])
	: begin_(0)
	, end_(N)
{
	static_assert(1 == sizeof(POD_TYPE), "invalid pod type(only 1 byte pod type)");

	buffer_.reset(new uint8[end_]);
	memcpy(buffer_.get(), buffer, N);
}

template <typename POD_TYPE>
shared_buffer::shared_buffer(POD_TYPE* buffer, uint32 size)
	: begin_(0)
	, end_(size)
{
	static_assert(1 == sizeof(POD_TYPE), "invalid pod type(only 1 byte pod type)");

	buffer_.reset(new uint8[end_]);
	memcpy(buffer_.get(), buffer, size);
}

inline boost::asio::const_buffers_1 buffer(const shared_buffer& b)
{
	return boost::asio::const_buffers_1(const_buffer(b.bytes(), b.size()));
}

inline boost::asio::mutable_buffers_1 buffer(shared_buffer& b)
{
	return boost::asio::mutable_buffers_1(mutable_buffer(const_cast<uint8*>(b.bytes()), b.size()));
}

}
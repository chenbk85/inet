#include "serialize_primitive.h"

namespace inet {

const_buffer load(const const_buffer& buffer, int16& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::load - not enough buffer");
	}

	v = ::ntohs(*(buffer_cast<const int16*>(buffer)));
	return buffer + sizeof(v);
}

const_buffer load(const const_buffer& buffer, int32& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::load - not enough buffer");
	}

	v = ::ntohl(*(buffer_cast<const int32*>(buffer)));
	return buffer + sizeof(v);
}

const_buffer load(const const_buffer& buffer, uint16& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::load - not enough buffer");
	}

	v = ::ntohs(*(buffer_cast<const uint16*>(buffer)));
	return buffer + sizeof(v);
}

const_buffer load(const const_buffer& buffer, uint32& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::load - not enough buffer");
	}

	v = ::ntohl(*(buffer_cast<const uint32*>(buffer)));
	return buffer + sizeof(v);
}



mutable_buffer save(const mutable_buffer& buffer, const int16& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::save - not enough buffer");
	}

	*(buffer_cast<int16*>(buffer)) = ::htons(v);
	return buffer + sizeof(v);
}

mutable_buffer save(const mutable_buffer& buffer, const int32& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::save - not enough buffer");
	}

	*(buffer_cast<int32*>(buffer)) = ::htonl(v);
	return buffer + sizeof(v);
}

mutable_buffer save(const mutable_buffer& buffer, const uint16& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::save - not enough buffer");
	}

	*(buffer_cast<uint16*>(buffer)) = ::htons(v);
	return buffer + sizeof(v);
}

mutable_buffer save(const mutable_buffer& buffer, const uint32& v)
{
	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::save - not enough buffer");
	}

	*(buffer_cast<uint32*>(buffer)) = ::htonl(v);
	return buffer + sizeof(v);
}

}
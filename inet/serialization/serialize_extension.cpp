#include "serialize_extension.h"

namespace inet {

const_buffer load(const_buffer& buffer, const_buffer& v)
{
	if(buffer_size(buffer) < buffer_size(v)) {
		throw serialization_error("not enough buffer");
	}

	memcpy(const_cast<uint8*>(buffer_cast<const uint8*>(v))
		, buffer_cast<const uint8*>(buffer), buffer_size(v));
	return buffer + buffer_size(v);
}

mutable_buffer save(mutable_buffer& buffer, const mutable_buffer& v)
{
	if(buffer_size(buffer) < buffer_size(v)) {
		throw serialization_error("not enough buffer");
	}

	memcpy(buffer_cast<uint8*>(buffer), buffer_cast<uint8*>(v), buffer_size(v));
	return buffer + buffer_size(v);
}

}
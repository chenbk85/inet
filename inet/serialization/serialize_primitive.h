#pragma once

namespace inet {

typedef std::length_error serialization_error;

template <typename T>
inline const_buffer load(const const_buffer& buffer, T& v)
{
	static_assert(boost::is_pod<T>::value, "not pod type. please implement serialize");

	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::load - not enough buffer");
	}

	v = *(buffer_cast<const T*>(buffer));
	return buffer + sizeof(v);
}

template <typename T>
inline mutable_buffer save(const mutable_buffer& buffer, const T& v)
{
	static_assert(boost::is_pod<T>::value, "not pod type. please implement serialize");

	if(buffer_size(buffer) < sizeof(v)) {
		throw serialization_error("inet::save - not enough buffer");
	}

	*(buffer_cast<T*>(buffer)) = v;
	return buffer + sizeof(v);
}


const_buffer load(const const_buffer& buffer, int16& v);
const_buffer load(const const_buffer& buffer, int32& v);
const_buffer load(const const_buffer& buffer, uint16& v);
const_buffer load(const const_buffer& buffer, uint32& v);


mutable_buffer save(const mutable_buffer& buffer, const int16& v);
mutable_buffer save(const mutable_buffer& buffer, const int32& v);
mutable_buffer save(const mutable_buffer& buffer, const uint16& v);
mutable_buffer save(const mutable_buffer& buffer, const uint32& v);

}
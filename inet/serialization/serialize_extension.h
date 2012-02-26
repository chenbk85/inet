#pragma once

namespace inet {

const_buffer load(const const_buffer& buffer, const_buffer& v);
mutable_buffer save(const mutable_buffer& buffer, const mutable_buffer& v);


template <typename T, uint32 N>
inline const_buffer load(const const_buffer& buffer, T (&data)[N])
{
	uint16 size = 0;
	const_buffer b = buffer & size;
	if(size != N) {
		throw serialization_error("inet::load - invalid array data size");
	}

	for(uint32 i = 0; i < N; ++i) {
		b = b & data[i];
	}

	return b;
}

template <typename T, uint32 N>
inline const_buffer load(const const_buffer& buffer, boost::array<T, N>& data)
{
	uint16 size = 0;
	const_buffer b = buffer & size;
	if(size != N) {
		throw serialization_error("inet::load - invalid array data size");
	}

	for(uint32 i = 0; i < N; ++i) {
		b = b & data[i];
	}

	return b;
}

template <typename Elem, typename Traits, typename Allocator,
	template<typename T, typename C, typename A> class String>
inline const_buffer load(const const_buffer& buffer, String<Elem, Traits, Allocator>& string)
{
	uint16 size = 0;
	const_buffer b = buffer & size;
	string.resize(size);

	for(auto iter = string.begin(); iter != string.end(); ++iter) {
		b = b & *iter;
	}

	return b;
}

template <typename Elem, typename Allocator,
	template<typename T, typename A> class Container>
inline const_buffer load(const const_buffer& buffer, Container<Elem, Allocator>& container)
{
	uint16 size = 0;
	const_buffer b = buffer & size;
	container.resize(size);
	
	for(auto iter = container.begin(); iter != container.end(); ++iter) {
		b = b & *iter;
	}

	return b;
}

template <typename T1, typename T2>
inline const_buffer load(const const_buffer& buffer, std::pair<T1, T2>& value)
{
	return buffer & value.first & value.second;
}

template <typename Key, typename Value, typename Compare, typename Allocator,
	template <typename K, typename V, typename C, typename A> class Container>
inline const_buffer load(const const_buffer& buffer, Container<Key, Value, Compare, Allocator>& container)
{
	uint16 size = 0;
	const_buffer b = buffer & size;
	
	for(uint16 i = 0; i < size; ++i) {
		Key k; Value v;
		b = b & k & v;

		container.insert(std::make_pair(k, v));
	}

	return b;
}

template <typename Key, typename Value, typename Hash, typename Pred, typename Allocator,
	template <typename K, typename T, typename H, typename P, typename A> class Container>
inline const_buffer load(const const_buffer& buffer, Container<Key, Value, Hash, Pred, Allocator>& container)
{
	uint16 size = 0;
	const_buffer b = buffer & size;
	
	for(uint16 i = 0; i < size; ++i) {
		Key k; Value v;
		b = b & k & v;

		container.insert(std::make_pair(k, v));
	}

	return b;
}





template <typename T, uint32 N>
inline mutable_buffer save(const mutable_buffer& buffer, const T (&data)[N])
{
	mutable_buffer b = buffer & static_cast<uint16>(N);
	for(uint32 i = 0; i < N; ++i) {
		b = b & data[i];
	}

	return b;
}

template <typename T, uint32 N>
inline mutable_buffer save(const mutable_buffer& buffer, const boost::array<T, N>& data)
{
	mutable_buffer b = buffer & static_cast<uint16>(N);
	for(uint32 i = 0; i < N; ++i) {
		b = b & data[i];
	}

	return b;
}

template <typename Elem, typename Traits, typename Allocator,
	template<typename T, typename C, typename A> class String>
inline mutable_buffer save(const mutable_buffer& buffer, const String<Elem, Traits, Allocator>& string)
{
	mutable_buffer b = buffer & static_cast<uint16>(string.length());

	for(auto iter = string.begin(); iter != string.end(); ++iter) {
		b = b & *iter;
	}

	return b;
}

template <typename Elem, typename Allocator,
	template<typename T, typename A> class Container>
inline mutable_buffer save(const mutable_buffer& buffer, const Container<Elem, Allocator>& container)
{
	mutable_buffer b = buffer & static_cast<uint16>(container.size());
	
	for(auto iter = container.begin(); iter != container.end(); ++iter) {
		b = b & *iter;
	}

	return b;
}

template <typename T1, typename T2>
inline mutable_buffer save(const mutable_buffer& buffer, const std::pair<T1, T2>& value)
{
	return buffer & value.first & value.second;
}

template <typename T1, typename T2>
inline mutable_buffer save(const mutable_buffer& buffer, const std::pair<const T1, T2>& value)
{
	return buffer & value.first & value.second;
}

template <typename Key, typename Value, typename Compare, typename Allocator,
	template<typename K, typename V, typename C, typename A> class Container>
inline mutable_buffer save(const mutable_buffer& buffer, const Container<Key, Value, Compare, Allocator>& container)
{
	mutable_buffer b = buffer & static_cast<uint16>(container.size());
	
	for(auto iter = container.begin(); iter != container.end(); ++iter) {
		b = b & *iter;
	}

	return b;
}

template <typename Key, typename Value, typename Hash, typename Pred, typename Allocator,
	template <typename K, typename T, typename H, typename P, typename A> class Container>
inline mutable_buffer save(const mutable_buffer& buffer, Container<Key, Value, Hash, Pred, Allocator>& container)
{
	mutable_buffer b = buffer & static_cast<uint16>(container.size());
	
	for(auto iter = container.begin(); iter != container.end(); ++iter) {
		b = b & *iter;
	}

	return b;
}

}
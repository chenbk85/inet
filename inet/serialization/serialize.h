#pragma once

namespace inet {
namespace tag {

struct serialize {};
struct serialize_split {};

}

template <typename T>
struct is_serialize
{
	static const bool value = boost::is_base_of<tag::serialize, T>::value;
};

template <typename T>
struct is_serialize_split
{
	static const bool value = boost::is_base_of<tag::serialize_split, T>::value;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename Archive>
struct serialize_member
{
	template <typename T>
	static Archive serialize(const Archive& buffer, T& v)
	{
		return v.serialize(buffer);
	}	
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename Archive>
struct serialize_member_split;

template <>
struct serialize_member_split<const_buffer>
{
	template <typename T>
	static const_buffer serialize(const const_buffer& buffer, T& v)
	{
		return v.load(buffer);
	}	
};

template <>
struct serialize_member_split<mutable_buffer>
{
	template <typename T>
	static mutable_buffer serialize(const mutable_buffer& buffer, T& v)
	{
		return v.save(buffer);
	}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename Archive>
struct serialize_primitive;

template <>
struct serialize_primitive<const_buffer>
{
	template <typename T>
	static const_buffer serialize(const const_buffer& buffer, T& v)
	{
		return load(buffer, v);
	}	
};

template <>
struct serialize_primitive<mutable_buffer>
{
	template <typename T>
	static mutable_buffer serialize(const mutable_buffer& buffer, T& v)
	{
		return save(buffer, v);
	}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <typename Archive, typename T>
inline Archive serialize(const Archive& ar, T& v)
{
	return boost::mpl::if_c<is_serialize<T>::value
			, serialize_member<Archive>
			, boost::mpl::if_c<is_serialize_split<T>::value
				, serialize_member_split<Archive>
				, serialize_primitive<Archive>>::type>
			::type::serialize(ar, v);
}

}


template <typename T>
inline inet::const_buffer operator& (const inet::const_buffer& ar, T& v)
{
	return inet::serialize(ar, v);
}

template <typename T>
inline inet::mutable_buffer operator& (const inet::mutable_buffer& ar, const T& v)
{
	return inet::serialize(ar, const_cast<T&>(v));
}

template <typename T>
inline inet::mutable_buffer operator& (const inet::mutable_buffer& ar, T& v)
{
	return inet::serialize(ar, v);
}
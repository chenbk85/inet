#pragma once

namespace inet {

template <typename T0>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0)
{
	return buffer & p0;
}

template <typename T0, typename T1>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1)
{
	return buffer & p0 & p1;
}

template <typename T0, typename T1, typename T2>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2)
{
	return buffer & p0 & p1 & p2;
}

template <typename T0, typename T1, typename T2, typename T3>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2, const T3& p3)
{
	return buffer & p0 & p1 & p2 & p3;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4)
{
	return buffer & p0 & p1 & p2 & p3 & p4;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7, const T8& p8)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7 & p8;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
inline mutable_buffer pack(const mutable_buffer& buffer, const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7, const T8& p8, const T9& p9)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7 & p8 & p9;
}



template <typename T0>
inline const_buffer unpack(const const_buffer& buffer, T0& p0)
{
	return buffer & p0;
}

template <typename T0, typename T1>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1)
{
	return buffer & p0 & p1;
}

template <typename T0, typename T1, typename T2>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2)
{
	return buffer & p0 & p1 & p2;
}

template <typename T0, typename T1, typename T2, typename T3>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2, T3& p3)
{
	return buffer & p0 & p1 & p2 & p3;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2, T3& p3, T4& p4)
{
	return buffer & p0 & p1 & p2 & p3 & p4;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6, T7& p7)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7;
}

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6, T7& p7, T8& p8)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7 & p8;
}
template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
inline const_buffer unpack(const const_buffer& buffer, T0& p0, T1& p1, T2& p2, T3& p3, T4& p4, T5& p5, T6& p6, T7& p7, T8& p8, T9& p9)
{
	return buffer & p0 & p1 & p2 & p3 & p4 & p5 & p6 & p7 & p8 & p9;
}

}
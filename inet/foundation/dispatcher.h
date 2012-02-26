#pragma once

#include <boost/unordered_map.hpp>

#include "delegate.h"

namespace inet {

template <typename ID, typename Delegate>
class dispatcher : public boost::noncopyable, public detail::slot_disconnector
{
public:
	typedef Delegate delegate_type;
	typedef typename delegate_type::function_type function_type;
	typedef boost::unordered_map<ID, delegate_type> container_type;

	boost::signals::connection add(const ID& id, function_type fun)
	{
		return container_[id].add(fun);
	}	

	bool dispatch(const ID& id)
	{
		static_assert(0 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second();
		return true;
	}

	template <typename P0>
	bool dispatch(const ID& id, const P0& p0)
	{
		static_assert(1 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0);
		return true;
	}

	template <typename P0, typename P1>
	bool dispatch(const ID& id, const P0& p0, const P1& p1)
	{
		static_assert(2 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1);
		return true;
	}

	template <typename P0, typename P1, typename P2>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2)
	{
		static_assert(3 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1, p2);
		return true;
	}

	template <typename P0, typename P1, typename P2, typename P3>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2, const P3& p3)
	{
		static_assert(4 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1, p2, p3);
		return true;
	}

	template <typename P0, typename P1, typename P2, typename P3, typename P4>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
	{
		static_assert(5 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1, p2, p3, p4);
		return true;
	}

	template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
	{
		static_assert(6 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1, p2, p3, p4, p5);
		return true;
	}

	template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6)
	{
		static_assert(7 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1, p2, p3, p4, p5, p6);
		return true;
	}

	template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7)
	{
		static_assert(8 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1, p2, p3, p4, p5, p6, p7);
		return true;
	}

	template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8)
	{
		static_assert(9 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}
		
		iter->second(p0, p1, p2, p3, p4, p5, p6, p7, p8);
		return true;
	}

	template <typename P0, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
	bool dispatch(const ID& id, const P0& p0, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5, const P6& p6, const P7& p7, const P8& p8, const P9& p9)
	{
		static_assert(10 == delegate_type::parameter_count, "number of parameters is incorrect.");

		auto iter = container_.find(id);
		if(iter == container_.end()) {
			return false;
		}

		iter->second(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		return true;
	}

private:
	container_type container_;
};

}
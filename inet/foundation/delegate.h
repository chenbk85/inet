#pragma once

#pragma warning(push)
#pragma warning(disable:4512)
#include <boost/signal.hpp>
#pragma warning(pop)

namespace inet {
namespace detail {

struct slot_disconnector
{
	void remove(const boost::signals::connection& slot)
	{
		slot.disconnect();
	}
};

}

template <typename T0 = void, typename T1 = void, typename T2 = void, typename T3 = void, typename T4 = void, 
		typename T5 = void, typename T6 = void, typename T7 = void, typename T8 = void, typename T9 = void>
class delegate : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&, const T7&, const T8&, const T9&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 10;

public:
	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}
	
	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7, const T8& p8, const T9& p9)
	{
		(*impl_)(p0, p1, p2, p3, p3, p5, p6, p7, p8, p9);
	}
	
private:
	impl_ptr impl_;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
class delegate<T0, T1, T2, T3, T4, T5, T6, T7, T8, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&, const T7&, const T8&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 9;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}
	
	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7, const T8& p8)
	{
		(*impl_)(p0, p1, p2, p3, p4, p5, p6, p7, p8);
	}
	
private:
	impl_ptr impl_;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
class delegate<T0, T1, T2, T3, T4, T5, T6, T7, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&, const T7&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 8;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}

	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6, const T7& p7)
	{
		(*impl_)(p0, p1, p2, p3, p4, p5, p6, p7);
	}

private:
	impl_ptr impl_;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
class delegate<T0, T1, T2, T3, T4, T5, T6, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&, const T3&, const T4&, const T5&, const T6&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 7;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}

	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5, const T6& p6)
	{
		(*impl_)(p0, p1, p2, p3, p4, p5, p6);
	}

private:
	impl_ptr impl_;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4, typename T5>
class delegate<T0, T1, T2, T3, T4, T5, void, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&, const T3&, const T4&, const T5&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 6;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}

	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4, const T5& p5)
	{
		(*impl_)(p0, p1, p2, p3, p4, p5);
	}

private:
	impl_ptr impl_;
};

template <typename T0, typename T1, typename T2, typename T3, typename T4>
class delegate<T0, T1, T2, T3, T4, void, void, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&, const T3&, const T4&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 5;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}

	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2, const T3& p3, const T4& p4)
	{
		(*impl_)(p0, p1, p2, p3, p4);
	}

private:
	impl_ptr impl_;
};

template <typename T0, typename T1, typename T2, typename T3>
class delegate<T0, T1, T2, T3, void, void, void, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&, const T3&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 4;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}

	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2, const T3& p3)
	{
		(*impl_)(p0, p1, p2, p3);
	}

private:
	impl_ptr impl_;
};

template <typename T0, typename T1, typename T2>
class delegate<T0, T1, T2, void, void, void, void, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&, const T2&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 3;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}
	
	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1, const T2& p2)
	{
		(*impl_)(p0, p1, p2);
	}
	
private:
	impl_ptr impl_;
};

template <typename T0, typename T1>
class delegate<T0, T1, void, void, void, void, void, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&, const T1&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 2;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}
	
	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0, const T1& p1)
	{
 		(*impl_)(p0, p1);
	}
	
private:
	impl_ptr impl_;
};

template <typename T0>
class delegate<T0, void, void, void, void, void, void, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void (const T0&)> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef typename impl_type::slot_type function_type;
	static const int parameter_count = 1;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}
	
	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() (const T0& p0)
	{
		(*impl_)(p0);
	}
	
private:
	impl_ptr impl_;
};

template <>
class delegate<void, void, void, void, void, void, void, void, void, void> : public detail::slot_disconnector
{
public:
	typedef boost::signal<void ()> impl_type;
	typedef boost::shared_ptr<impl_type> impl_ptr;
	typedef impl_type::slot_type function_type;
	static const int parameter_count = 0;

	delegate()
		: impl_(new impl_type)
	{
	}

	delegate(const function_type& fun)
		: impl_(new impl_type)
	{
		add(fun);
	}

	boost::signals::connection add(const function_type& fun)
	{
		return impl_->connect(fun);
	}
	
	delegate operator+= (const function_type& fun)
	{
		add(fun);
		return *this;
	}

	delegate operator= (const function_type& fun)
	{
		impl_.reset(new impl_type);
		add(fun);
		return *this;
	}

	void operator() ()
	{
		(*impl_)();
	}
	
private:
	impl_ptr impl_;
};

}
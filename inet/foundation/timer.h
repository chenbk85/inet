#pragma once

#include <boost/asio.hpp>

namespace inet {

class timer : public boost::noncopyable
{
public:
	explicit timer(boost::asio::io_service& io_service);
	virtual ~timer();

	virtual void async_wait(boost::function<void()> fun, duration expiry_time);
	virtual void wait(duration expiry_time);

protected:
	boost::scoped_ptr<boost::asio::basic_waitable_timer<clock>> core_;
};

typedef boost::shared_ptr<timer> timer_ptr;
typedef boost::weak_ptr<timer> timer_handle;

}
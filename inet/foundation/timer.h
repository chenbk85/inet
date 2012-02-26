#pragma once

#include <boost/asio.hpp>

namespace inet {

class timer : public boost::noncopyable, public boost::enable_shared_from_this<timer>
{
public:
	explicit timer(boost::asio::io_service& io_service);
	virtual ~timer();

	virtual void start(boost::function<bool()> fun, uint32 expiry_time);
	virtual void restart();

protected:
	virtual void on_timeout(const boost::system::error_code& e);

protected:
	boost::scoped_ptr<boost::asio::deadline_timer> core_;

	boost::function<bool()> fun_;
	boost::posix_time::time_duration expiry_time_;
};

typedef boost::shared_ptr<timer> timer_ptr;

}
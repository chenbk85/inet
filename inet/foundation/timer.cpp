#include "timer.h"

namespace inet {

timer::timer(boost::asio::io_service& io_service)
	: core_(new boost::asio::deadline_timer(io_service))
{
}

timer::~timer()
{
}

void timer::start(boost::function<bool()> fun, uint32 expiry_time)
{
	fun_ = fun;
	expiry_time_ = boost::posix_time::milliseconds(expiry_time);

	restart();
}

void timer::restart()
{
	boost::weak_ptr<timer> handle = shared_from_this();

	core_->expires_from_now(expiry_time_);
	core_->async_wait([handle](const boost::system::error_code& e) {
		if(timer_ptr pointer = handle.lock())
			pointer->on_timeout(e);
	});
}

void timer::on_timeout(const boost::system::error_code& e)
{
	if(e) {
		return;
	}

	if(fun_()) {
		restart();
	} else {
		fun_ = [] { return false; };
	}
}


}
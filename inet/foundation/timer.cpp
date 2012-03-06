#include "timer.h"

namespace inet {

timer::timer(boost::asio::io_service& io_service)
	: core_(new boost::asio::basic_waitable_timer<clock>(io_service))
{
}

timer::~timer()
{
}

void timer::async_wait(boost::function<void()> fun, duration expiry_time)
{
	core_->expires_from_now(expiry_time);
	core_->async_wait([=] (const boost::system::error_code& e) {
		if(!e) {
			fun();
		}
	});
}

void timer::wait(duration expiry_time)
{
	core_->expires_from_now(expiry_time);
	core_->wait();
}


}
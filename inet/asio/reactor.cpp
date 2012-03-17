#include "reactor.h"

#include "acceptor.h"
#include "session.h"

namespace inet {
namespace asio {

reactor::reactor()
	: io_service_(new boost::asio::io_service)
{
}

reactor::~reactor()
{
}

void reactor::poll()
{
	io_service_->poll();
}

void reactor::run()
{
	work_.reset(new boost::asio::io_service::work(*io_service_));
	io_service_->run();
}

void reactor::set_end()
{
	work_.reset();
	io_service_->stop();
}

bool reactor::stopped() const
{
	return io_service_->stopped();
}

boost::asio::io_service& reactor::get_io_service()
{
	return *io_service_;
}

inet::acceptor_ptr reactor::add_acceptor(inet::acceptor_ptr acceptor)
{
	acceptor_list_.insert(acceptor);
	return acceptor;
}

inet::timer_handle reactor::add_timer(inet::timer_ptr timer)
{
	timer_list_.insert(timer);
	return timer;
}

void reactor::remove_timer(inet::timer_handle handle)
{
	timer_list_.erase(handle.lock());
}



}
}
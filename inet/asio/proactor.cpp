#include "proactor.h"

#include "acceptor.h"
#include "session.h"

namespace inet {
namespace asio {

proactor::proactor(uint32 thread_count)
	: io_service_(new inet::io_thread_service(thread_count))
{
}

proactor::proactor(uint32 thread_count, boost::function<bool()> startup)
	: io_service_(new inet::io_thread_service(thread_count, startup))
{
}

proactor::~proactor()
{
	set_end();
	wait_end();
}

void proactor::set_end()
{
	while(!acceptor_list_.empty())
	{
		acceptor_ptr acceptor;
		acceptor_list_.pop(acceptor);
		acceptor->close();
	}

	io_service_->set_force_end();
}

void proactor::wait_end()
{
	io_service_->wait_end();
}

boost::asio::io_service& proactor::get_io_service()
{
	return *io_service_;
}

inet::acceptor_ptr proactor::add_acceptor(inet::acceptor_ptr acceptor)
{
	acceptor_list_.push(acceptor);
	return acceptor;
}

inet::timer_handle proactor::add_timer(inet::timer_ptr timer)
{
	timer_list_type::accessor acc;
	if(timer_list_.insert(acc, timer.get()))
	{
		return acc->second = timer;
	}

	return inet::timer_handle();
}

void proactor::remove_timer(inet::timer_handle handle)
{
	timer_list_.erase(handle.lock().get());
}


}}
#include "io_service.h"

namespace inet {
namespace asio {
namespace detail {

uint32 concurrency_thread_count(uint32 size)
{
	return size == 0 ? boost::thread::hardware_concurrency() : size;
}
}//namespace detail


io_service::io_service(uint32 thread_count)
	: thread_count_(detail::concurrency_thread_count(thread_count))
	, startup_([]{})
{
}

io_service::io_service(boost::function<void()> startup, uint32 thread_count)
	: thread_count_(detail::concurrency_thread_count(thread_count))
	, startup_(startup)
{
}

io_service::~io_service()
{
	set_end();
	wait_end();
}

void io_service::run()
{
	work_.reset(new boost::asio::io_service::work(core_));
	for(uint32 i = 0; i < thread_count_; ++i) {
		thread_pool_.create_thread([&]{ startup_(); core_.run();});
	}
}

void io_service::set_end()
{
	work_.reset();
}

void io_service::set_force_end()
{
	work_.reset();
	core_.stop();
}

void io_service::wait_end()
{
	thread_pool_.join_all();
}

uint32 io_service::size() const
{
	return thread_count_;
}

io_service::operator boost::asio::io_service& ()
{
	return core_;
}

}}
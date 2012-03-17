#include "io_thread_service.h"

#include "timer.h"

namespace inet {
namespace detail {

void thread_main(boost::asio::io_service& io_service
	, boost::function<bool()> startup
	, boost::function<void()> cleanup)
{
	if(startup() == false) {
		return;
	}

	io_service.run();

	cleanup();
}

uint32 concurrency_thread_count(uint32 size)
{
	return size == 0 ? boost::thread::hardware_concurrency() * 2 : size;
}
}//namespace detail


io_thread_service::io_thread_service(uint32 size
	, boost::function<bool()> startup
	, boost::function<void()> cleanup)
	: work_(new boost::asio::io_service::work(io_service_))
{
	for(uint32 i = 0; i < detail::concurrency_thread_count(size) ; ++i) {
		pool_.create_thread(boost::bind(&detail::thread_main
		, boost::ref(io_service_), startup, cleanup));
	}
}

io_thread_service::io_thread_service(uint32 size
	, boost::function<bool()> startup)
	: work_(new boost::asio::io_service::work(io_service_))
{
	for(uint32 i = 0; i < detail::concurrency_thread_count(size); ++i) {
		pool_.create_thread(boost::bind(&detail::thread_main
		, boost::ref(io_service_), startup, []{}));
	}
}

io_thread_service::io_thread_service(uint32 size)
	: work_(new boost::asio::io_service::work(io_service_))
{
	for(uint32 i = 0; i < detail::concurrency_thread_count(size); ++i) {
		pool_.create_thread(boost::bind(&detail::thread_main
		, boost::ref(io_service_), []{return true;}, []{}));
	}
}

io_thread_service::~io_thread_service()
{
	set_end();
	wait_end();
}

void io_thread_service::post(boost::function<void()> job)
{
	io_service_.post(job);
}

uint32 io_thread_service::size() const
{
	return pool_.size();
}

void io_thread_service::set_end()
{
	work_.reset();
}

void io_thread_service::set_force_end()
{
	work_.reset();
	io_service_.stop();
}

void io_thread_service::wait_end()
{
	pool_.join_all();
}

io_thread_service::operator boost::asio::io_service& ()
{
	return io_service_;
}

}
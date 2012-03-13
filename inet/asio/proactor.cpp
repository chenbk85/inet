#include "proactor.h"

#include "acceptor.h"
#include "session.h"

namespace inet {
namespace asio {

proactor::proactor(uint32 thread_count)
	: io_service_(new inet::io_thread_service(thread_count))
{
}

proactor::proactor(boost::function<bool()> startup, uint32 thread_count)
	: io_service_(new inet::io_thread_service(thread_count, startup))
{
}

proactor::~proactor()
{
	set_end();
	wait_end();
}

inet::connector_ptr proactor::connect(const end_point& endpoint)
{
	auto session = create_connector();
	session->connect(endpoint);

	return session;
}

inet::connector_ptr proactor::connect(const end_point& endpoint, boost::function<void ()> connect_handler)
{
	auto session = connect(endpoint);
	session->on_connect = connect_handler;

	return session;
}

inet::acceptor_ptr proactor::listen(uint16 port)
{
	auto acceptor = create_acceptor();
	acceptor->listen(port);
	acceptor->post_accept();

	acceptor_list_.push(acceptor);
	return acceptor;
}

inet::acceptor_ptr proactor::listen(uint16 port, boost::function<void (session_ptr)> connection_handler)
{
	auto acceptor = listen(port);
	acceptor->on_connection = connection_handler;

	return acceptor;
}

timer_handle proactor::set_timeout(boost::function<void()> cb, duration expiry_time)
{
	timer_ptr t = create_timer();
	tbb::concurrent_hash_map<timer*, timer_ptr>::accessor acc;
	if(timer_list_.insert(acc, t.get()))
	{
		timer_handle handle = t;
		t->async_wait([=] {
			if(!handle.expired()) {
				cb();
				clear_timeout(handle);
			}
		}, expiry_time);

		acc->second = t;
		return handle;
	}

	return timer_handle();
}

void proactor::clear_timeout(timer_handle handle)
{
	timer_list_.erase(handle.lock().get());
}

timer_handle proactor::set_interval(boost::function<void()> cb, duration expiry_time)
{
	timer_ptr t = create_timer();
	tbb::concurrent_hash_map<timer*, timer_ptr>::accessor acc;
	if(timer_list_.insert(acc, t.get()))
	{
		timer_handle handle = t;
		boost::function<void()> handler;
		handler = [=] {
			if(handle.lock()) {
				cb();
				handle.lock()->async_wait(handler, expiry_time);
			}
		};
		t->async_wait(handler, expiry_time);

		acc->second = t;
		return handle;
	}

	return timer_handle();
}

void proactor::clear_interval(timer_handle handle)
{
	timer_list_.erase(handle.lock().get());
}

void proactor::set_end()
{
	while(!acceptor_list_.empty())
	{
		acceptor_ptr acceptor;
		acceptor_list_.pop(acceptor);
		acceptor->close();
	}

	io_service_->set_end();
}

void proactor::set_force_end()
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

acceptor_ptr proactor::create_acceptor()
{
	boost::shared_ptr<asio::acceptor> acceptor(new asio::acceptor);
	acceptor->open(*io_service_, [=] { return create_session(); });

	return acceptor;
}

connector_ptr proactor::create_connector()
{
	boost::shared_ptr<asio::session> session(new asio::session);
	session->open(*io_service_);

	return session;
}

session_ptr proactor::create_session()
{
	return create_connector();
}

timer_ptr proactor::create_timer()
{
	return timer_ptr(new timer(*io_service_));
}


}}
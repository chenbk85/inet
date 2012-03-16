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

connector_ptr reactor::connect(const end_point& endpoint)
{
	auto session = create_connector();
	session->connect(endpoint);

	return session;
}

connector_ptr reactor::connect(const end_point& endpoint, boost::function<void ()> connect_handler)
{
	auto session = connect(endpoint);
	session->on_connect = connect_handler;

	return session;
}

acceptor_ptr reactor::listen(uint16 port)
{
	auto acceptor = create_acceptor();
	acceptor->listen(port);
	acceptor->post_accept();

	acceptor_list_.insert(acceptor);
	return acceptor;
}

acceptor_ptr reactor::listen(uint16 port, boost::function<void (session_ptr)> connection_handler)
{
	auto acceptor = listen(port);
	acceptor->on_connection = connection_handler;

	return acceptor;
}

timer_handle reactor::set_timeout(boost::function<void()> cb, duration expiry_time)
{
	timer_ptr t = create_timer();
	if(timer_list_.insert(t).second)
	{
		timer_handle handle = t;
		t->async_wait([=] {
			if(!handle.expired()) {
				cb();
				clear_timeout(handle);
			}
		}, expiry_time);

		return handle;
	}

	return timer_handle();
}

void reactor::clear_timeout(timer_handle handle)
{
	timer_list_.erase(handle.lock());
}

timer_handle reactor::set_interval(boost::function<void()> cb, duration expiry_time)
{
	timer_ptr t = create_timer();
	if(timer_list_.insert(t).second)
	{
		timer_handle handle = t;
		boost::function<void()> handler;
		handler = [=] {
			if(timer_ptr timer = handle.lock()) {
				cb();
				timer->async_wait(handler, expiry_time);
			}
		};
		t->async_wait(handler, expiry_time);

		return handle;
	}

	return timer_handle();
}

void reactor::clear_interval(timer_handle handle)
{
	clear_timeout(handle);
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

acceptor_ptr reactor::create_acceptor()
{
	boost::shared_ptr<asio::acceptor> acceptor(new asio::acceptor);
	acceptor->open(*io_service_, [=] { return create_session(); });

	return acceptor;
}

connector_ptr reactor::create_connector()
{
	boost::shared_ptr<asio::session> session(new asio::session);
	session->open(*io_service_);

	return session;
}

session_ptr reactor::create_session()
{
	return create_connector();
}

timer_ptr reactor::create_timer()
{
	return timer_ptr(new timer(*io_service_));
}

}
}
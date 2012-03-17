#pragma once

#include <boost/type_traits.hpp>

namespace inet {
namespace asio {

template <typename T>
class io_service : public T
{
	static_assert(boost::is_base_of<inet::io_service, T>::value, "T must inherit inet::io_service.");

public:
	virtual ~io_service()
	{
	}

	virtual connector_ptr connect(const end_point& endpoint)
	{
		auto session = create_connector();
		session->connect(endpoint);

		return session;
	}

	virtual connector_ptr connect(const end_point& endpoint, boost::function<void ()> connect_handler)
	{
		auto session = connect(endpoint);
		session->on_connect = connect_handler;

		return session;
	}

	virtual acceptor_ptr listen(uint16 port)
	{
		auto acceptor = create_acceptor();
		acceptor->listen(port);
		acceptor->post_accept();

		return add_acceptor(acceptor);
	}

	virtual acceptor_ptr listen(uint16 port, boost::function<void (session_ptr)> connection_handler)
	{
		auto acceptor = listen(port);
		acceptor->on_connection = connection_handler;

		return acceptor;
	}

	virtual timer_handle set_timeout(boost::function<void()> cb, duration expiry_time)
	{
		timer_ptr timer = create_timer();

		timer_handle handle = timer;
		timer->async_wait([=] {
			if(!handle.expired()) {
				cb();
				clear_timeout(handle);
			}
		}, expiry_time);

		return add_timer(timer);
	}

	virtual void clear_timeout(timer_handle handle)
	{
		remove_timer(handle);
	}

	virtual timer_handle set_interval(boost::function<void()> cb, duration expiry_time)
	{
		timer_ptr timer = create_timer();

		timer_handle handle = timer;
		boost::function<void()> handler;
		handler = [=] {
			if(timer_ptr t = handle.lock()) {
				cb();
				t->async_wait(handler, expiry_time);
			}
		};
		timer->async_wait(handler, expiry_time);

		return add_timer(timer);
	}

	virtual void clear_interval(timer_handle handle)
	{
		remove_timer(handle);
	}

protected:
	virtual acceptor_ptr create_acceptor()
	{
		boost::shared_ptr<asio::acceptor> acceptor(new asio::acceptor);
		acceptor->open(get_io_service(), [=] { return create_session(); });

		return acceptor;
	}

	virtual connector_ptr create_connector()
	{
		boost::shared_ptr<asio::session> session(new asio::session);
		session->open(get_io_service());

		return session;
	}

	virtual session_ptr create_session()
	{
		return create_connector();
	}

	virtual timer_ptr create_timer()
	{
		return timer_ptr(new inet::timer(get_io_service()));
	}

protected:
	virtual boost::asio::io_service& get_io_service() = 0;
	virtual inet::acceptor_ptr add_acceptor(inet::acceptor_ptr acceptor) = 0;
	virtual inet::timer_handle add_timer(inet::timer_ptr timer) = 0;
	virtual void remove_timer(inet::timer_handle handle) = 0;
};

}
}
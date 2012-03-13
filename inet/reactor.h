#pragma once

namespace inet {

struct reactor
{
	virtual connector_ptr connect(const end_point& endpoint) = 0;
	virtual connector_ptr connect(const end_point& endpoint, boost::function<void ()> connect_handler) = 0;
	virtual acceptor_ptr listen(uint16 port) = 0;
	virtual acceptor_ptr listen(uint16 port, boost::function<void (session_ptr)> connection_handler) = 0;

	virtual timer_handle set_timeout(boost::function<void()> cb, duration expiry_time) = 0;
	virtual void clear_timeout(timer_handle handle) = 0;
	virtual timer_handle set_interval(boost::function<void()> cb, duration expiry_time) = 0;
	virtual void clear_interval(timer_handle handle) = 0;

	virtual void run() = 0;
	virtual void poll() = 0;
};

typedef boost::shared_ptr<reactor> reactor_ptr;

}
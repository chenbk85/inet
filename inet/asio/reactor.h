#pragma once

#include <boost/unordered_set.hpp>

namespace inet {
namespace asio {

class reactor : public inet::reactor
{
public:
	reactor();
	virtual ~reactor();

	virtual connector_ptr connect(const end_point& endpoint);
	virtual connector_ptr connect(const end_point& endpoint, boost::function<void ()> connect_handler);
	virtual acceptor_ptr listen(uint16 port);
	virtual acceptor_ptr listen(uint16 port, boost::function<void (session_ptr)> connection_handler);

	virtual timer_handle set_timeout(boost::function<void()> cb, duration expiry_time);
	virtual void clear_timeout(timer_handle handle);
	virtual timer_handle set_interval(boost::function<void()> cb, duration expiry_time);
	virtual void clear_interval(timer_handle handle);

	virtual void poll();

	virtual void run();
	virtual void set_end();
	virtual bool stopped() const;

protected:
	virtual acceptor_ptr create_acceptor();
	virtual connector_ptr create_connector();
	virtual session_ptr create_session();
	virtual timer_ptr create_timer();

protected:
	boost::scoped_ptr<boost::asio::io_service> io_service_;
	boost::scoped_ptr<boost::asio::io_service::work> work_;

	boost::unordered_set<acceptor_ptr> acceptor_list_;
	boost::unordered_set<timer_ptr> timer_list_;
};

}
}
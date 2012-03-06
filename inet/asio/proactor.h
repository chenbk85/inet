#pragma once

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_hash_map.h>

namespace inet {
namespace asio {

class proactor : public inet::proactor
{
public:
	explicit proactor(uint32 thread_count = 0);
	proactor(boost::function<bool()> startup, uint32 thread_count);
	virtual ~proactor();

	virtual inet::connector_ptr connect(const end_point& endpoint);
	virtual inet::connector_ptr connect(const end_point& endpoint, boost::function<void ()> connect_handler);
	virtual inet::acceptor_ptr listen(uint16 port);
	virtual inet::acceptor_ptr listen(uint16 port, boost::function<void (session_ptr)> connection_handler);
	
	virtual timer_handle set_timeout(boost::function<void()> cb, duration expiry_time);
	virtual void clear_timeout(timer_handle handle);
	virtual timer_handle set_interval(boost::function<void()> cb, duration expiry_time);
	virtual void clear_interval(timer_handle handle);

	virtual void set_end();
	virtual void set_force_end();
	virtual void wait_end();

protected:
	virtual acceptor_ptr create_acceptor();
	virtual connector_ptr create_connector();
	virtual session_ptr create_session();
	virtual timer_ptr create_timer();

protected:
	boost::scoped_ptr<inet::io_thread_service> io_service_;
	tbb::concurrent_bounded_queue<acceptor_ptr> acceptor_list_;
	tbb::concurrent_hash_map<timer*, timer_ptr> timer_list_;
};

}}
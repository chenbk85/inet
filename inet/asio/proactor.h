#pragma once

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_hash_map.h>

#include "io_service.h"

namespace inet {
namespace asio {

class proactor : public io_service<inet::proactor>
{
public:
	explicit proactor(uint32 thread_count = 0);
	explicit proactor(uint32 thread_count, boost::function<bool()> startup);
	virtual ~proactor();

	virtual void set_end();
	virtual void wait_end();

protected:
	virtual boost::asio::io_service& get_io_service();
	virtual inet::acceptor_ptr add_acceptor(inet::acceptor_ptr acceptor);
	virtual inet::timer_handle add_timer(inet::timer_ptr timer);
	virtual void remove_timer(inet::timer_handle handle);

protected:
	boost::scoped_ptr<inet::io_thread_service> io_service_;
	tbb::concurrent_bounded_queue<acceptor_ptr> acceptor_list_;

	typedef tbb::concurrent_hash_map<inet::timer*, inet::timer_ptr> timer_list_type;
	timer_list_type timer_list_;
};

}}
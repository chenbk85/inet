#pragma once

#include "io_service.h"

namespace inet {
namespace asio {

class proactor : public inet::proactor
{
public:
	explicit proactor(uint32 thread_count = 0);
	proactor(boost::function<void()> startup, uint32 thread_count);
	virtual ~proactor();

	virtual inet::session_ptr connect(const end_point& endpoint);
	virtual inet::session_ptr connect(const end_point& endpoint, boost::function<void (session_ptr)> fun);
	virtual void listen(uint16 port);
	virtual timer_ptr create_timer();

	virtual void run();

	virtual void set_end();
	virtual void set_force_end();
	virtual void wait_end();

protected:
	virtual acceptor_ptr create_acceptor();
	virtual connector_ptr create_connector();
	virtual session_ptr create_session();

protected:
	asio::io_service io_service_;
	std::list<acceptor_ptr> acceptor_list_;
};

}}
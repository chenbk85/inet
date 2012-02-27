#pragma once

#include "io_service.h"

namespace inet {

struct proactor : io_service
{
	virtual connector_ptr connect(const end_point& endpoint) = 0;
	virtual connector_ptr connect(const end_point& endpoint, boost::function<void ()> connect_handler) = 0;
	virtual acceptor_ptr listen(uint16 port) = 0;
	virtual acceptor_ptr listen(uint16 port, boost::function<void (session_ptr)> connection_handler) = 0;
	virtual timer_ptr create_timer() = 0;
};

typedef boost::shared_ptr<proactor> proactor_ptr;

}
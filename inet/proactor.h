#pragma once

#include "io_service.h"
#include "factory.h"

namespace inet {

struct proactor : io_service, factory
{
	virtual session_ptr connect(const end_point& endpoint) = 0;
	virtual session_ptr connect(const end_point& endpoint, boost::function<void (session_ptr)> fun) = 0;
	virtual void listen(uint16 port) = 0;
	virtual timer_ptr create_timer() = 0;
};

}
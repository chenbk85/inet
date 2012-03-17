#pragma once

#include "io_service.h"

namespace inet {

struct reactor : io_service
{
	virtual void poll() = 0;
	virtual void run() = 0;
	virtual void set_end() = 0;
	virtual bool stopped() const = 0;
};

typedef boost::shared_ptr<reactor> reactor_ptr;

}
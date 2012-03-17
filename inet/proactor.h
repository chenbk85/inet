#pragma once

#include "io_service.h"

namespace inet {

struct proactor : io_service
{
	virtual void set_end() = 0;
	virtual void wait_end() = 0;
};

typedef boost::shared_ptr<proactor> proactor_ptr;

}
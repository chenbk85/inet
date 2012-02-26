#pragma once

#include "end_point.h"
#include "session.h"

namespace inet {

struct connector : session
{
	virtual void connect(const end_point& endpoint) = 0;

	delegate<void> on_connect;
};

typedef boost::shared_ptr<connector> connector_ptr;

}
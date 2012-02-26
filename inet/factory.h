#pragma once

#include "acceptor.h"
#include "session.h"
#include "connector.h"

namespace inet {

struct factory
{
	virtual ~factory() {}

	virtual acceptor_ptr create_acceptor() = 0;
	virtual connector_ptr create_connector() = 0;
	virtual session_ptr create_session() = 0;
};

}
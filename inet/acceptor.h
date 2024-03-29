#pragma once

#include "session.h"

namespace inet {

struct acceptor
{
	virtual ~acceptor() {}

	virtual void listen(uint16 port) = 0;
	virtual void post_accept() = 0;
	virtual void close() = 0;

	delegate<session_ptr> on_connection;
	delegate<error> on_error;
	delegate<void> on_close;
};

typedef boost::shared_ptr<acceptor> acceptor_ptr;

}
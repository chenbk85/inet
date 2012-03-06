#pragma once

#include "error.h"
#include "end_point.h"

namespace inet {

struct session
{
	virtual ~session() {}

	virtual void close() = 0;
	virtual void send(const shared_buffer& buffer) = 0;
	virtual end_point local_endpoint() = 0;
	virtual end_point remote_endpoint() = 0;

	delegate<shared_buffer> on_data;
	delegate<error> on_error;
	delegate<void> on_close;
};

typedef boost::shared_ptr<session> session_ptr;
typedef boost::weak_ptr<session> session_handle;

}
#pragma once

#include "dispatcher_helper.h"

namespace inet {
namespace protobuf {

class dispatcher
{
public:
	typedef inet::dispatcher<std::string, inet::delegate<shared_buffer>> dispatcher_type;

	virtual ~dispatcher() {}

	template <typename T, typename Handler>
	boost::signals::connection add(Handler handler)
	{
		return core_.add(T::descriptor()->name(), dispatcher_helper<T, Handler>(handler));
	}

	virtual bool dispatch(shared_buffer buffer)
	{
		std::string id;
		auto remain_buffer = unpack(inet::buffer(buffer), id);
		buffer.remove(buffer.size() - inet::buffer_size(remain_buffer));
		return core_.dispatch(id, buffer);
	}

protected:
	dispatcher_type core_;
};

}
}
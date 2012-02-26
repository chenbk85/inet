#pragma once

namespace inet {
namespace protobuf {

template <typename T, typename Handler>
struct dispatcher_helper
{
	explicit dispatcher_helper(Handler handler)
		: handler_(handler)
	{
	}

	void operator() (const shared_buffer& buffer)
	{
		auto message = to_message<T>(buffer);
		handler_(*message);
	}

private:
	Handler handler_;
};

}
}
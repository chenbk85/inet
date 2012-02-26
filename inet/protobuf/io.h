#pragma once

namespace inet {
namespace protobuf {

typedef std::runtime_error stream_error;

namespace detail {

inline shared_buffer make_buffer(const google::protobuf::Message& message)
{
	auto descriptor = message.GetDescriptor();
	const std::string& name = descriptor->name();

	uint16 buffer_size = static_cast<uint16>(sizeof(uint16) + name.length() + message.ByteSize());
	shared_buffer result(buffer_size);

	auto remain_buffer = inet::buffer(result.bytes(), result.size()) & name;
	return result.remove(buffer_size - inet::buffer_size(remain_buffer));
}

}


inline shared_buffer to_shared_buffer(const google::protobuf::Message& message)
{
	shared_buffer buffer = detail::make_buffer(message);
	google::protobuf::io::ArrayOutputStream ostream(buffer.bytes(), buffer.size());

	if(!message.SerializeToZeroCopyStream(&ostream)) {
		throw stream_error("google protobuf write error or message is missing required fields");
	}

	return buffer.restore();
}

template <typename T>
inline boost::shared_ptr<T> to_message(const shared_buffer& buffer)
{
	boost::shared_ptr<T> message(new T);
	google::protobuf::io::ArrayInputStream istream(buffer.bytes(), buffer.size());

	if(!message->ParseFromZeroCopyStream(&istream)) {
		throw stream_error("google protobuf read error or input is in the wrong format");
	}

	return message;
}

}
}
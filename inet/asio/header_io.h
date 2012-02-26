#pragma once

namespace inet {

enum { k_packet_header_size = sizeof(uint16) };

inline shared_buffer make_header(const shared_buffer& buffer)
{
	shared_buffer result(k_packet_header_size);
	inet::pack(inet::buffer(result), static_cast<uint16>(buffer.size()));
	return result;
}

inline uint16 packet_payload_size(shared_buffer& buffer, std::size_t bytes_transferred)
{
	buffer.remove(bytes_transferred);
	if(buffer.empty())
	{
		buffer.restore();

		uint16 size = 0;
		inet::unpack(inet::buffer(buffer), size);
		return size;
	}

	return 0;
}

}
#pragma once

namespace inet {

struct end_point
{
	end_point(const std::string& addr, uint16 port_num)
		: address(addr), port(port_num) {
	}
	
	std::string address;
	uint16 port;
};

}
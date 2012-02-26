#pragma once

namespace inet {

struct io_service
{
	virtual ~io_service() {}

	virtual void run() = 0;

	virtual void set_end() = 0;
	virtual void set_force_end() = 0;
	virtual void wait_end() = 0;
};

}
#pragma once

#include <deque>
#include "shared_buffer.h"

namespace inet {

class shared_buffer_list
{
public:
	shared_buffer_list();
	explicit shared_buffer_list(const shared_buffer& buffer);
	virtual ~shared_buffer_list();

	void push(const shared_buffer& buffer);
	void remove(uint32 size);
	uint32 size() const;
	bool empty() const;

	friend std::vector<mutable_buffer> buffer(shared_buffer_list& );

protected:
	std::deque<shared_buffer> buffer_list_;
};


std::vector<mutable_buffer> buffer(shared_buffer_list& bl);

}
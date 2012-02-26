#include "shared_buffer_list.h"

namespace inet {

shared_buffer_list::shared_buffer_list()
{
}

shared_buffer_list::shared_buffer_list(const shared_buffer& buffer)
	: buffer_list_(1, buffer)
{
}

shared_buffer_list::~shared_buffer_list()
{
}

void shared_buffer_list::push(const shared_buffer& buffer)
{
	buffer_list_.push_back(buffer);
}

void shared_buffer_list::remove(uint32 size)
{
	while(0 < size && !buffer_list_.empty()) {
		shared_buffer& buffer = buffer_list_.front();
		if(buffer.size() <= size) {
			size -= buffer.size();
			buffer_list_.pop_front();
		} else {
			buffer.remove(size);
			size = 0;
		}
	}
}

uint32 shared_buffer_list::size() const
{
	return buffer_list_.size();
}

bool shared_buffer_list::empty() const
{
	return buffer_list_.empty();
}

std::vector<mutable_buffer> buffer(shared_buffer_list& bl)
{
	std::vector<mutable_buffer> result;
	result.reserve(bl.size());

	for(auto iter = bl.buffer_list_.begin(); iter != bl.buffer_list_.end(); ++iter) {
		result.push_back(inet::buffer(*iter));
	}

	return result;
}

}
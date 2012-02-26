#pragma once

namespace inet {

struct layer
{
	virtual ~layer() {}

	virtual shared_buffer encode(shared_buffer& buffer) = 0;
	virtual shared_buffer decode(shared_buffer& buffer) = 0;
};

typedef boost::shared_ptr<layer> layer_ptr;

}
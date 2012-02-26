#pragma once

namespace inet {

struct error
{
	virtual ~error() {}

	virtual uint32 code() const = 0;
	virtual std::string message() const = 0;
	virtual operator bool () const = 0;
};

}
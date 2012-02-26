#pragma once

namespace inet {
namespace asio {

class error : public inet::error
{
public:
	error();
	explicit error(boost::system::error_code error_code);
	virtual ~error();

	virtual uint32 code() const;
	virtual std::string message() const;
	virtual operator bool () const;

private:
	boost::system::error_code error_code_;
};

}}
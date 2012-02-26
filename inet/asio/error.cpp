#include "error.h"

namespace inet {
namespace asio {

error::error()
{
}

error::error(boost::system::error_code error_code)
	: error_code_(error_code)
{
}

error::~error()
{
}

uint32 error::code() const
{
	return error_code_.value();
}

std::string error::message() const
{
	return error_code_.message();
}

error::operator bool () const
{
	return error_code_ 
		&& boost::asio::error::eof != code()
		&& boost::asio::error::operation_aborted != code()
		&& boost::asio::error::connection_reset != code()
		&& boost::asio::error::bad_descriptor != code();
}

}}
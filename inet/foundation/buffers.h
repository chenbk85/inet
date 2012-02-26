#pragma once

#include <boost/asio/buffer.hpp>

namespace inet {

typedef boost::asio::mutable_buffer mutable_buffer;
typedef boost::asio::const_buffer const_buffer;

using boost::asio::buffer;
using boost::asio::buffer_cast;
using boost::asio::buffer_size;

}
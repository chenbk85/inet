#include "session.h"
#include "error.h"
#include "header_io.h"

#include <boost/lexical_cast.hpp>

namespace inet {
namespace asio {

using namespace boost::asio::ip;

session::session()
	: sending_(false)
	, received_header_(k_packet_header_size)
{
}

session::~session()
{
}

void session::connect(const end_point& endpoint)
{
	auto this_ptr = shared_from_this();
	strand_->post([=] {
		tcp::resolver resolver(this_ptr->socket_->get_io_service());
		tcp::resolver::query query(tcp::v4(), endpoint.address, boost::lexical_cast<std::string>(endpoint.port));

		this_ptr->socket_->async_connect(*(resolver.resolve(query)), this_ptr->strand_->wrap([=] (const boost::system::error_code& ec) {
			if(!ec) {
				this_ptr->on_connect();
				this_ptr->post_receive();
				return;
			}

			this_ptr->on_error(error(ec));
		}));
	});
}

void session::close()
{
	auto this_ptr = shared_from_this();
	strand_->post([=] {
		boost::system::error_code ec;
		this_ptr->socket_->shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
	});
}

void session::send(const inet::shared_buffer& buffer)
{
	auto this_ptr = shared_from_this();
	strand_->post([=] {
		this_ptr->send_buffer_list_.push(make_header(buffer));
		this_ptr->send_buffer_list_.push(buffer);

		this_ptr->send_flush();
	});
}

end_point session::local_endpoint()
{
	auto endpoint = socket_->local_endpoint();
	return end_point(endpoint.address().to_string(), endpoint.port());
}

end_point session::remote_endpoint()
{
	auto endpoint = socket_->remote_endpoint();
	return end_point(endpoint.address().to_string(), endpoint.port());
}

void session::open(boost::asio::io_service& io_service)
{
	strand_.reset(new boost::asio::io_service::strand(io_service));
	socket_.reset(new tcp::socket(io_service));
}

void session::post_receive()
{
	auto this_ptr = shared_from_this();
	strand_->post([=] {
		this_ptr->socket_->async_read_some(inet::buffer(this_ptr->received_header_)
			, this_ptr->strand_->wrap([=] (const boost::system::error_code& ec, std::size_t bytes_transferred) {
				if(!ec) {
					uint16 size = packet_payload_size(this_ptr->received_header_, bytes_transferred);
					if(size == 0) {
						this_ptr->post_receive();
						return;
					}

					this_ptr->post_receive_payload(shared_buffer(size));
					return;
				} else if(error(ec)) {
					this_ptr->on_error(error(ec));
				}

				this_ptr->on_close();
			})
		);
	});
}

tcp::socket& session::socket()
{
	return *socket_;
}

void session::send_flush()
{
	if(!sending_ && !send_buffer_list_.empty())
	{
		auto this_ptr = shared_from_this();
		socket_->async_write_some(inet::buffer(send_buffer_list_)
			, strand_->wrap([=] (const boost::system::error_code& ec, std::size_t bytes_transferred) {
				if(!ec) {
					this_ptr->sending_ = false;
					this_ptr->send_buffer_list_.remove(bytes_transferred);

					this_ptr->send_flush();
					return;
				} else if(error(ec)) {
					this_ptr->on_error(error(ec));
				}
				
				this_ptr->on_close();
			})
		);

		sending_ = true;
	}
}

void session::post_receive_payload(shared_buffer buffer)
{
	auto this_ptr = shared_from_this();
	socket_->async_read_some(inet::buffer(buffer)
		, strand_->wrap([=] (const boost::system::error_code& ec, std::size_t bytes_transferred) mutable {
			if(!ec) {
				buffer.remove(bytes_transferred);
				if(0 < buffer.size()) {
					this_ptr->post_receive_payload(buffer);
					return;
				}

				buffer.restore();
				this_ptr->on_data(buffer);
				this_ptr->post_receive();
				return;
			} else if(error(ec)) {
				this_ptr->on_error(error(ec));
			}
			
			this_ptr->on_close();
		})
	);
}

}}
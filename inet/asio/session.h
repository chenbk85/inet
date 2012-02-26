#pragma once

namespace inet {
namespace asio {

class session : public inet::connector,
				public boost::enable_shared_from_this<session>
{
public:
	session();
	virtual ~session();

	virtual void connect(const end_point& endpoint);
	virtual void close();
	virtual void send(const inet::shared_buffer& buffer);

	virtual end_point local_endpoint();
	virtual end_point remote_endpoint();

	void open(boost::asio::io_service& io_service);
	void post_receive();

	boost::asio::ip::tcp::socket& socket();

private:
	void send_flush();
	void post_receive_payload(shared_buffer buffer);

private:
	boost::scoped_ptr<boost::asio::io_service::strand> strand_;
	boost::scoped_ptr<boost::asio::ip::tcp::socket> socket_;

	bool sending_;
	shared_buffer_list send_buffer_list_;
	shared_buffer received_header_;
};

}}
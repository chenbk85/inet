#pragma once

namespace inet {
namespace asio {

class acceptor : public inet::acceptor,
				 public boost::enable_shared_from_this<acceptor>
{
public:
	acceptor();
	virtual ~acceptor();

	virtual void listen(uint16 port);
	virtual void post_accept();

	virtual void close();
	void open(boost::asio::io_service& io_service
		, boost::function<inet::session_ptr(void)> gen_session);

protected:
	boost::scoped_ptr<boost::asio::io_service::strand> strand_;
	boost::scoped_ptr<boost::asio::ip::tcp::acceptor> core_;
		
	boost::function<inet::session_ptr(void)> gen_session_;
};

}}
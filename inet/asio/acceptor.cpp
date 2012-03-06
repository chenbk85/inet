#include "acceptor.h"

#include "error.h"
#include "session.h"

namespace inet {
namespace asio {

using namespace boost::asio::ip;

acceptor::acceptor()
{
}

acceptor::~acceptor()
{
}

void acceptor::listen(uint16 port)
{
	try {
		tcp::endpoint endpoint(tcp::v4(), port);
		core_->open(endpoint.protocol());
		core_->bind(endpoint);
		core_->set_option(tcp::acceptor::reuse_address(true));
		core_->listen();
	} catch (boost::system::system_error& e) {
		on_error(error(e.code()));
	}
}

void acceptor::post_accept()
{
	auto this_ptr = shared_from_this();
	strand_->post([=] {
		if(!core_->is_open()) {
			this_ptr->on_close();
			return;
		}

		auto new_session = boost::static_pointer_cast<asio::session>(this_ptr->gen_session_());
		this_ptr->core_->async_accept(new_session->socket(), this_ptr->strand_->wrap([=] (const boost::system::error_code& ec) {
			this_ptr->post_accept();
			if(!ec) {
				this_ptr->on_connection(new_session);
				new_session->post_receive();
			} else if(error(ec)) {
				this_ptr->on_error(error(ec));
			}
		}));
	});
}

void acceptor::close()
{
	auto this_ptr = shared_from_this();
	strand_->post([=] {
		this_ptr->core_->close();
	});
}

void acceptor::open(boost::asio::io_service& io_service
	, boost::function<inet::session_ptr(void)> gen_session)
{
	core_.reset(new tcp::acceptor(io_service));
	strand_.reset(new boost::asio::io_service::strand(io_service));
	gen_session_ = gen_session;
}


}}
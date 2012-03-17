#pragma once

#include <boost/unordered_set.hpp>

#include "io_service.h"

namespace inet {
namespace asio {

class reactor : public io_service<inet::reactor>
{
public:
	reactor();
	virtual ~reactor();

	virtual void poll();
	virtual void run();
	virtual void set_end();
	virtual bool stopped() const;

protected:
	virtual boost::asio::io_service& get_io_service();
	virtual inet::acceptor_ptr add_acceptor(inet::acceptor_ptr acceptor);
	virtual inet::timer_handle add_timer(inet::timer_ptr timer);
	virtual void remove_timer(inet::timer_handle handle);

protected:
	boost::scoped_ptr<boost::asio::io_service> io_service_;
	boost::scoped_ptr<boost::asio::io_service::work> work_;

	boost::unordered_set<acceptor_ptr> acceptor_list_;
	boost::unordered_set<timer_ptr> timer_list_;
};

}
}
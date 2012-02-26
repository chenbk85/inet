#pragma once

#include <boost/asio.hpp>

namespace inet {

class io_thread_service : public boost::noncopyable
{
public:
	io_thread_service(uint32 size, boost::function<bool()> startup, boost::function<void()> cleanup);
	io_thread_service(uint32 size, boost::function<bool()> startup);
	explicit io_thread_service(uint32 size = 0);
	~io_thread_service();

	void post(boost::function<void()> job);
	void post(boost::function<void()> job, uint32 expiry_time);

	uint32 size() const;

	void set_end();
	void set_force_end();
	void wait_end();

	operator boost::asio::io_service& ();

private:
	boost::asio::io_service io_service_;
	boost::scoped_ptr<boost::asio::io_service::work> work_;

	boost::thread_group pool_;
};

}
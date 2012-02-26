#pragma once

namespace inet {
namespace asio {

class io_service : public inet::io_service
{
public:
	explicit io_service(uint32 thread_count = 0);
	io_service(boost::function<void()> startup, uint32 thread_count);
	virtual ~io_service();

	virtual void run();

	virtual void set_end();
	virtual void set_force_end();
	virtual void wait_end();

	uint32 size() const;
	operator boost::asio::io_service& ();

protected:
	boost::asio::io_service core_;
	boost::scoped_ptr<boost::asio::io_service::work> work_;

	boost::thread_group thread_pool_;

private:
	boost::function<void()> startup_;
	uint32 thread_count_;
};

}}
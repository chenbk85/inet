#pragma once

#include <boost/thread.hpp>

namespace inet {

template <typename T>
class singleton : public boost::noncopyable
{
public:
	static T& instance()
	{
		if(!instance_) {
			boost::mutex::scoped_lock lock(mutex_);
			if(!instance_) {
				instance_.reset(new T);
			}
		}

		return *instance_;
	}

	static void release()
	{
		boost::mutex::scoped_lock lock(mutex_);
		instance_.reset();
	}
	
private:
	static boost::mutex mutex_;
	static boost::scoped_ptr<T> instance_;
};

template <typename T>
boost::mutex singleton<T>::mutex_;

template <typename T>
boost::scoped_ptr<T> singleton<T>::instance_;

}
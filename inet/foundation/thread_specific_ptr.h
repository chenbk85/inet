#pragma once

namespace inet {

template <typename T>
class thread_specific_ptr : boost::noncopyable
{
public:
	static T& instance()
	{
		if(instance_.get() == nullptr) {
			instance_.reset(new T);
		}

		return *instance_;
	}

	static void acquire(T* instance)
	{
		instance_.reset(instance);
	}

	static T* release()
	{
		return instance_.release();
	}
	
private:
	static boost::thread_specific_ptr<T> instance_;
};

template <typename T>
boost::thread_specific_ptr<T> thread_specific_ptr<T>::instance_;

}
#pragma once

#ifdef _DEBUG
#define TBB_USE_DEBUG 1
#endif
#include <tbb/tbbmalloc_proxy.h>
#include <tbb/atomic.h>

#include <string>
#include <vector>

#define BOOST_HAS_RVALUE_REFS 1

#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "type.h"
#include "buffers.h"
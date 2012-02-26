#pragma once

#include <inet/inet.h>
#include <inet/serialization/serialization.h>

#pragma warning(push)
#pragma warning(disable:4512)
#pragma warning(disable:4244)
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#pragma warning(pop)

#include "io.h"
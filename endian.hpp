#ifndef included_596bb6f0_d143_43d5_bf8e_cc2635f210f0
#define included_596bb6f0_d143_43d5_bf8e_cc2635f210f0
#pragma once

//#if defined(_BSD_SOURCE)
//#include <endian.h>
//#if 1 //if defined(_DARWIN_SOURCE)
#include <libkern/OSByteOrder.h>
//#endif
//#endif

#include <bassoon/bson.hpp>

namespace bassoon {
  namespace bson {
    // BSON stores integers in little endian format.
//#ifdef _BSD_SOURCE
//#if 1 defined(__DARWIN_SOURCE)
    inline int32_t host_to_little_endian(int32_t value) { return OSSwapHostToLittleInt32(value); }
    inline int64_t host_to_little_endian(int64_t value) { return OSSwapHostToLittleInt64(value); }
    inline int32_t little_endian_to_host(int32_t value) { return OSSwapLittleToHostInt32(value); }
    inline int64_t little_endian_to_host(int64_t value) { return OSSwapLittleToHostInt64(value); }
// #else
//     inline int32_t host_to_little_endian(int32_t value) { return htole32(value); }
//     inline int64_t host_to_little_endian(int64_t value) { return htole64(value); }
//     inline int32_t little_endian_to_host(int32_t value) { return le32toh(value); }
//     inline int64_t little_endian_to_host(int64_t value) { return le64toh(value); }
// #endif
// #else
// #error "bassoon: I don't know how to do byte order on this platform!"
// #endif

  }  // namespace bson
}  // namespace bassoon

#endif // included_596bb6f0_d143_43d5_bf8e_cc2635f210f0

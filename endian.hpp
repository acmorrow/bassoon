#ifndef included_596bb6f0_d143_43d5_bf8e_cc2635f210f0
#define included_596bb6f0_d143_43d5_bf8e_cc2635f210f0
#pragma once

#if defined(_BSD_SOURCE)
#include <endian.h>
#endif // _BSD_SOURCE

#include <bassoon/bson.hpp>

namespace bassoon {
  namespace bson {

    // BSON stores integers in little endian format.
#ifdef _BSD_SOURCE
    inline int32_t host_to_little_endian(int32_t value) { return htole32(value); }
    inline int64_t host_to_little_endian(int64_t value) { return htole64(value); }
    inline int32_t little_endian_to_host(int32_t value) { return le32toh(value); }
    inline int64_t little_endian_to_host(int64_t value) { return le64toh(value); }
#else
#error "bassoon: I don't know how to do byte order on this platform!"
#endif

  }  // namespace bson
}  // namespace bassoon

#endif // included_596bb6f0_d143_43d5_bf8e_cc2635f210f0

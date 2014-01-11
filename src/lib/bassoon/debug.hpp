#ifndef included_17daf617_6d77_4138_9909_4a6bc4522369
#define included_17daf617_6d77_4138_9909_4a6bc4522369

#include <cstdlib>
#include <iosfwd>

#include <bassoon/export.hpp>

namespace bassoon {
  namespace bson {

#if defined(NDEBUG)
    const int k_debug = 0;
#else
    const int k_debug = 1;
#endif  // NDEBUG

    ///
    /// Dumps 'length' number of bytes from 'data' to the stream 'stream'. The format
    /// is \\xXX where XX is a hex code.
    ///
    void LIBBASSOON_EXPORT dump_bytes_as_hex(
        std::ostream& stream,
        void const* data, std::size_t length);

  } // namespace bson
} // namespace bassoon

#endif // included_17daf617_6d77_4138_9909_4a6bc4522369

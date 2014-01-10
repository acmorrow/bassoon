#ifndef included_17daf617_6d77_4138_9909_4a6bc4522369
#define included_17daf617_6d77_4138_9909_4a6bc4522369

#include <cstdlib>
#include <iosfwd>

#include <bassoon/export.h>

namespace bassoon {
  namespace bson {

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

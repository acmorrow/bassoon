#ifndef included_17daf617_6d77_4138_9909_4a6bc4522369
#define included_17daf617_6d77_4138_9909_4a6bc4522369

#include <iosfwd>

namespace bassoon {

  /// Dumps 'length' number of bytes from 'data' to the stream 'stream'. The format
  /// is \\xXX where XX is a hex code.
  void dump_bytes_as_hex(std::ostream& stream, void const* data, std::size_t length);

} // namespace bassoon

#endif // included_17daf617_6d77_4138_9909_4a6bc4522369

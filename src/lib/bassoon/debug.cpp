#include <bassoon/debug.hpp>


namespace bassoon {

  /// Dumps 'length' number of bytes from 'data' to the stream 'stream'. The format
  /// is \\xXX where XX is a hex code.
  void dump_bytes_as_hex(std::ostream& stream, void const* data, std::size_t length) {
    static const char items[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };

    byte_t const* cdata = reinterpret_cast<byte_t const*>(data);
    for (std::size_t i = 0; i != length; ++i) {
      stream << "\\x" << items[(cdata[i] & 0xf0) >> 4] << items[(cdata[i] & 0x0f)];
    }
  }

} // namespace bassoon

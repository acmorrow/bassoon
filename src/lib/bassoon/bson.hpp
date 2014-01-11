#ifndef included_0d05e359_97ab_4ce9_a6a8_113d7f68113d
#define included_0d05e359_97ab_4ce9_a6a8_113d7f68113d

#include <cinttypes>
#include <climits>
#include <type_traits>

#include <bassoon/export.hpp>

namespace bassoon {
  namespace bson {

    using byte_t   = uint8_t;
    using length_t = int32_t;
    using double_t = double;

    static_assert(
        CHAR_BIT == 8,
        "bassoon: requires 8 bit bytes");

    static_assert(
      sizeof(double_t) == 8,
      "bassoon: fundamental double type is not 8 bytes");

    enum class values : byte_t {
      null   = 0x0,
      false_ = 0x0,
      true_  = 0x1
    };

    enum class types : byte_t {
      floating_point                    = 0x01,
      utf8_string                       = 0x02,
      document                          = 0x03,
      array                             = 0x04,
      binary                            = 0x05,
      undefined_no_deprecated           = 0x06,
      undefined LIBBASSOON_DEPRECATED   = undefined_no_deprecated,
      object_id                         = 0x07,
      boolean                           = 0x08,
      utc_datetime                      = 0x09,
      null                              = 0x0A,
      regex                             = 0x0B,
      db_pointer_no_deprecated          = 0x0C,
      db_pointer LIBBASSOON_DEPRECATED  = db_pointer_no_deprecated,
      javascript                        = 0x0D,
      symbol                            = 0x0E,
      scoped_javascript                 = 0x0F,
      int32                             = 0x10,
      timestamp                         = 0x11,
      int64                             = 0x12,
      min                               = 0xFF,
      max                               = 0x7F
    };

    enum class binary_subtypes : byte_t {
      generic   = 0x00,
      function  = 0x01,
      oldbinary = 0x02,
      uuid      = 0x03,
      md5       = 0x05,
      user      = 0x80
    };

    const std::size_t k_object_id_length = 12U;

  }  // namespace bson
}  // namespace bassoon

#endif  // included_0d05e359_97ab_4ce9_a6a8_113d7f68113d

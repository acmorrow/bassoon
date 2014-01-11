#ifndef included_0d05e359_97ab_4ce9_a6a8_113d7f68113d
#define included_0d05e359_97ab_4ce9_a6a8_113d7f68113d

#include <cinttypes>
#include <type_traits>

#include <bassoon/bassoon.h>
#include <bassoon/bson.h>

namespace bassoon {
  namespace bson {

    using byte_t   = bassoon_bson_byte_t;
    using length_t = bassoon_bson_length_t;
    using double_t = bassoon_bson_double_t;

    static_assert(
      std::is_same<std::uint8_t, byte_t>::value,
      "bassoon: fundamental type for 'byte' does not match C++11 std::uint8_t type");

    static_assert(
      sizeof(length_t) == 4,
      "bassoon: fundamental length type is not 4 bytes");

    static_assert(
      std::is_signed<length_t>::value,
      "bassoon: fundamental length type is not signed");

    static_assert(
      sizeof(double_t) == 8,
      "bassoon: fundamental double type is not 8 bytes");

    enum class values : byte_t {
      null   = bassoon_bson_null_byte,
      false_ = bassoon_bson_false_byte,
      true_  = bassoon_bson_true_byte
    };

    enum class types : byte_t {
      floating_point                    = bassoon_bson_type_floating_point,
      utf8_string                       = bassoon_bson_type_utf8_string,
      document                          = bassoon_bson_type_document,
      array                             = bassoon_bson_type_array,
      binary                            = bassoon_bson_type_binary,
      undefined_no_deprecated           = bassoon_bson_type_undefined_no_deprecated,
      undefined BASSOON_DEPRECATED_API  = undefined_no_deprecated,
      object_id                         = bassoon_bson_type_object_id,
      boolean                           = bassoon_bson_type_boolean,
      utc_datetime                      = bassoon_bson_type_utc_datetime,
      null                              = bassoon_bson_type_null,
      regex                             = bassoon_bson_type_regex,
      db_pointer_no_deprecated          = bassoon_bson_type_db_pointer_no_deprecated,
      db_pointer BASSOON_DEPRECATED_API = db_pointer_no_deprecated,
      javascript                        = bassoon_bson_type_javascript,
      symbol                            = bassoon_bson_type_symbol,
      scoped_javascript                 = bassoon_bson_type_scoped_javascript,
      int32                             = bassoon_bson_type_int32,
      timestamp                         = bassoon_bson_type_timestamp,
      int64                             = bassoon_bson_type_int64,
      min                               = bassoon_bson_type_min,
      max                               = bassoon_bson_type_max
    };

    enum class binary_subtypes : byte_t {
      generic   = bassoon_bson_binary_subtype_generic,
      function  = bassoon_bson_binary_subtype_function,
      oldbinary = bassoon_bson_binary_subtype_oldbinary,
      uuid      = bassoon_bson_binary_subtype_uuid,
      md5       = bassoon_bson_binary_subtype_md5,
      user      = bassoon_bson_binary_subtype_user
    };

    const std::size_t k_object_id_length = bassoon_bson_k_object_id_length;

    const bool k_debug = (bassoon_bson_k_debug != 0);

  }  // namespace bson
}  // namespace bassoon

#endif  // included_0d05e359_97ab_4ce9_a6a8_113d7f68113d

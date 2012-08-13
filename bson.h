#ifndef included_39aff450_bcc8_43ef_bcce_7f70a51b2623
#define included_39aff450_bcc8_43ef_bcce_7f70a51b2623
#pragma once

#include <inttypes.h>
#include <bassoon/bassoon.h>

#if defined(__cplusplus)
extern "C" {
#endif  // __cplusplus

// The fundamental types declared in the bson spec.
typedef uint8_t bassoon_bson_byte_t;
typedef double  bassoon_bson_double_t;

// BSON lengths are signed 32 bit.
typedef int32_t bassoon_bson_length_t;

// The fundamental values declared in the bson spec.
const bassoon_bson_byte_t bassoon_bson_null_byte  = 0x00;
const bassoon_bson_byte_t bassoon_bson_false_byte = 0x00;
const bassoon_bson_byte_t bassoon_bson_true_byte  = 0x01;

// The fundamental type tags declared in the bson spec.
const bassoon_bson_byte_t bassoon_bson_type_floating_point                    = 0x01;
const bassoon_bson_byte_t bassoon_bson_type_utf8_string                       = 0x02;
const bassoon_bson_byte_t bassoon_bson_type_document                          = 0x03;
const bassoon_bson_byte_t bassoon_bson_type_array                             = 0x04;
const bassoon_bson_byte_t bassoon_bson_type_binary                            = 0x05;
const bassoon_bson_byte_t bassoon_bson_type_undefined_no_deprecated           = 0x06;
const bassoon_bson_byte_t bassoon_bson_type_undefined BASSOON_DEPRECATED_API  = bassoon_bson_type_undefined_no_deprecated;
const bassoon_bson_byte_t bassoon_bson_type_object_id                         = 0x07;
const bassoon_bson_byte_t bassoon_bson_type_boolean                           = 0x08;
const bassoon_bson_byte_t bassoon_bson_type_utc_datetime                      = 0x09;
const bassoon_bson_byte_t bassoon_bson_type_null                              = 0x0A;
const bassoon_bson_byte_t bassoon_bson_type_regex                             = 0x0B;
const bassoon_bson_byte_t bassoon_bson_type_db_pointer_no_deprecated          = 0x0C;
const bassoon_bson_byte_t bassoon_bson_type_db_pointer BASSOON_DEPRECATED_API = bassoon_bson_type_db_pointer_no_deprecated;
const bassoon_bson_byte_t bassoon_bson_type_javascript                        = 0x0D;
const bassoon_bson_byte_t bassoon_bson_type_symbol                            = 0x0E;
const bassoon_bson_byte_t bassoon_bson_type_scoped_javascript                 = 0x0F;
const bassoon_bson_byte_t bassoon_bson_type_int32                             = 0x10;
const bassoon_bson_byte_t bassoon_bson_type_timestamp                         = 0x11;
const bassoon_bson_byte_t bassoon_bson_type_int64                             = 0x12;
const bassoon_bson_byte_t bassoon_bson_type_min                               = 0xFF;
const bassoon_bson_byte_t bassoon_bson_type_max                               = 0x7F;

const bassoon_bson_byte_t bassoon_bson_binary_subtype_generic   = 0x00;
const bassoon_bson_byte_t bassoon_bson_binary_subtype_function  = 0x01;
const bassoon_bson_byte_t bassoon_bson_binary_subtype_oldbinary = 0x02;
const bassoon_bson_byte_t bassoon_bson_binary_subtype_uuid      = 0x03;
const bassoon_bson_byte_t bassoon_bson_binary_subtype_md5       = 0x05;
const bassoon_bson_byte_t bassoon_bson_binary_subtype_user      = 0x80;

const size_t bassoon_bson_k_object_id_length = 12;

#if defined(NDEBUG)
const int bassoon_bson_k_debug = 0;
#else
const int bassoon_bson_k_debug = 1;
#endif  // NDEBUG

#if defined(__cplusplus)
}  // __cplusplus
#endif  // __cplusplus

#endif  // included_39aff450_bcc8_43ef_bcce_7f70a51b2623

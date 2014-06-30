#ifndef included_4eacc70e_583d_4044_895e_271ed97bdb63
#define included_4eacc70e_583d_4044_895e_271ed97bdb63

#include <bassoon/bassoon.hpp>
#include <bassoon/bson.hpp>
#include <bassoon/string_data.hpp>

namespace bassoon {
  namespace bson {

    /// This abstract class describes the interface to the low level
    /// BSON encoder. Typically, this will be used via its concrete
    /// subclass, 'encoder<T>', which is final and therefore
    /// devirtualizes. If you need to pass an encoder somewhere, but
    /// don't want to deal with the templated aspect, you can use the
    /// the encoder_interface to work around. You pay some performance
    /// penalty due to the virtual functions and needing dynamic
    /// allocations to create subdocuments.
    ///
    /// You must call 'finish' on the abstract encoder when you are
    /// done with it, the destructor does not do it. If you want
    /// automatic behavior, use the RAII document builders described
    /// in document.hpp.
    ///
    /// If you create subdocuments or subarrays, you must treat the
    /// parent encoder as immutable until you have called 'finish' on
    /// the subdocument or subarrays.
    ///
    /// There must be no more than one unfinished subdocument or
    /// subarray active on a given encoder_interface.

    class LIBBASSOON_EXPORT encoder_interface {
    public:
      virtual ~encoder_interface() = default;

      ///
      /// Returns true if our underlying writer is 'ok'.
      ///
      virtual bool ok() const = 0;

      ///
      /// Encodes the double 'value' into the current document as 'name'.
      ///
      virtual encoder_interface& encode_floating_point(cstring_cdata name, double_t value) = 0;

      ///
      /// Encodes the string 'value' into the current document as 'name'.
      ///
      virtual encoder_interface& encode_utf8_string(cstring_cdata name, string_cdata value) = 0;

      ///
      /// Encodes 'subdocument' into the current document as
      /// 'name'. The method assumes that 'data' points to a valid
      /// BSON document, which means that the first four bytes are a
      /// little endian size and that the last byte is null, so we
      /// don't need the size as an argument. The data is copied.
      ///
      ///
      // TODO(acm): Make subdocument a type that can check for errors in debug mode.
      virtual encoder_interface& encode_subdocument(cstring_cdata name, void const* subdocument) = 0;

      ///
      /// Encodes 'data', which is not in document style yet (no
      /// leading length, no trailing null, but is otherwise valid
      /// BSON) into the current document as 'name'. The data is
      /// copied.
      ///
      virtual encoder_interface& encode_as_subdocument(cstring_cdata name, binary_cdata data) = 0;

      ///
      /// Encodes 'subarray' into the current document as 'name'. The
      /// method assumes that 'data' points to a valid BSON array
      /// document, which means that the first four bytes are a little
      /// endian size and that the last byte is null, so we don't need
      /// the size as an argument. The data is copied.
      ///
      virtual encoder_interface& encode_subarray(cstring_cdata name, void const* subarray) = 0;

      ///
      /// Encodes 'data', which is not in array style yet (no
      /// leading length, no trailing null, but is otherwise a valid
      /// BSON array) into the current document as 'name'. The data is
      /// copied.
      ///
      virtual encoder_interface& encode_as_subarray(cstring_cdata name, binary_cdata data) = 0;

      ///
      /// Encodes the provided binary data into the current docuemtn
      /// as 'name', tagged with the given 'subtype'.
      ///
      virtual encoder_interface& encode_binary(cstring_cdata name, binary_subtypes subtype, binary_cdata data) = 0;

      ///
      /// Encodes an undefined element into the current document as
      /// 'name'.
      ///
      virtual encoder_interface& encode_undefined(cstring_cdata name) LIBBASSOON_DEPRECATED = 0;

      ///
      /// Encodes the given object id 'id' into the current document
      /// as 'name'.
      ///
      virtual encoder_interface& encode_object_id(cstring_cdata name, object_id_cdata id) = 0;

      ///
      /// Encodes the given boolean value 'value' into the current
      /// document as 'name'.
      ///
      virtual encoder_interface& encode_boolean(cstring_cdata name, bool value) = 0;

      ///
      /// Encodes the given utc datetime value, interpreted as
      /// milliseconds from the epoch, into the current document as
      /// 'name'.
      ///
      virtual encoder_interface& encode_utc_datetime(cstring_cdata name, std::int64_t value) = 0;

      ///
      /// Encodes a null element into the current document as 'name'.
      ///
      virtual encoder_interface& encode_null(cstring_cdata name) = 0;

      ///
      /// Encodes the given regex 'regex' into the current document as
      /// 'name'. The contents of 'options' are as documented in the
      /// BSON spec.
      ///
      virtual encoder_interface& encode_regex(cstring_cdata name, cstring_cdata regex, cstring_cdata options) = 0;

      ///
      /// Encodes the given name 'dbname' and object id 'id' into the
      /// current document as 'name'.
      ///
      virtual encoder_interface& encode_db_pointer(cstring_cdata name, string_cdata dbname, object_id_cdata id) LIBBASSOON_DEPRECATED = 0;

      ///
      /// Encode the given string data 'code' as javascript into the
      /// current document as 'name'.
      ///
      virtual encoder_interface& encode_javascript(cstring_cdata name, string_cdata code) = 0;

      ///
      /// Encode the given string data 'symbol' as a symbol in the
      /// current document as 'name'.
      ///
      virtual encoder_interface& encode_symbol(cstring_cdata name, string_cdata symbol) = 0;

      ///
      /// Encode the provided javascript code 'code', along with the
      /// provided 'scope' subdocument, as a "javascript with scope"
      /// object in the current document, as 'name'. Note that this
      /// copies the scope document. There is currently no support for
      /// zero-copy scope subdocuments. The pointed to scope document
      /// must be complete (i.e. has a leading length, and a trailing
      /// null byte).
      ///
      virtual encoder_interface& encode_scoped_javascript(cstring_cdata name, string_cdata code, void const* scope) = 0;

      ///
      /// Encode the given integer value 'value' into the current document
      /// as 'name'
      ///
      virtual encoder_interface& encode_int32(cstring_cdata name, std::int32_t value) = 0;

      ///
      /// Encode the given integer value 'value' as a timestamp in the current
      /// document as 'name'.
      ///
      virtual encoder_interface& encode_timestamp(cstring_cdata name, std::int64_t value) = 0;

      ///
      /// Encode the given integer 'value' into the current document
      /// as 'name'.
      ///
      virtual encoder_interface& encode_int64(cstring_cdata name, std::int64_t value) = 0;

      ///
      /// Encode a minimum key value into the current document as 'name'.
      ///
      virtual encoder_interface& encode_min_key(cstring_cdata name) = 0;

      ///
      /// Encode a minimum key value into the current document as 'name'.
      ///
      virtual encoder_interface& encode_max_key(cstring_cdata name) = 0;

    protected:
      encoder_interface() = default;
    };

    class abstract_encoder : public encoder_interface {
    public:
        virtual ~abstract_encoder() = default;

        ///
        /// Starts a new subdocument with the name 'name'.
        ///
        virtual abstract_encoder& start_subdocument(cstring_cdata name) = 0;

        ///
        /// Starts a new subarray with the name 'name'.
        ///
        virtual abstract_encoder& start_subarray(cstring_cdata name) = 0;

        ///
        /// Finishes the current document. You must call this once for every call
        /// to 'start_subdocument' or 'start_subarray', plus one extra time to
        /// finish the top-level document.
        ///
        virtual abstract_encoder& finish() = 0;

    protected:
        abstract_encoder() = default;
    };

  }  // namespace bson
}  // namespace bassoon

#endif // included_4eacc70e_583d_4044_895e_271ed97bdb63

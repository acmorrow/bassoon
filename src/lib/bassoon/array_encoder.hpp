#ifndef included_4c0e4038_bea4_448f_8d89_127de0ccd9c2
#define included_4c0e4038_bea4_448f_8d89_127de0ccd9c2

#include <bassoon/encoder.hpp>

namespace bassoon {
  namespace bson {

    namespace details {
      template<typename encoder_type>
      struct encoder_storage {
        typedef encoder_type& type;
      };

      template<typename writer_type>
      struct encoder_storage<encoder<writer_type>> {
        typedef encoder<writer_type> type;
      };
    } // namespace details


    class LIBBASSOON_EXPORT array_encoder_base {
    protected:
      array_encoder_base()
        : index_() {}

      const char* index() {
        // TODO: Make this fast
        sprintf(work_, "%u", index_++);
        return work_;
      }

    private:
      unsigned int index_;
      char work_[128];
    };

    template<typename encoder_type>
    class array_encoder : array_encoder_base {

    public:
      explicit array_encoder(encoder_type& encoder)
        : encoder_(encoder) {}

      bool ok() const {
        return encoder_.ok();
      }

      array_encoder& encode_floating_point(double_t value) {
        encoder_.encode_floating_point(index(), value);
        return *this;
      }

      array_encoder& encode_utf8_string(string_cdata value) {
        encoder_.encode_utf8_string(index(), value);
        return *this;
      }

      array_encoder& encode_subdocument(void const* subdocument) {
        encoder_.encode_subdocument(index(), subdocument);
        return *this;
      }

      array_encoder& encode_as_subdocument(binary_cdata data) {
        encoder_.encode_as_subdocument(index(), data);
        return *this;
      }

      array_encoder& encode_subarray(void const* subarray) {
        encoder_.encode_subarray(index(), subarray);
        return *this;
      }

      array_encoder& encode_as_subarray(binary_cdata data) {
        encoder_.encode_as_subarray(index(), data);
        return *this;
      }

      array_encoder& encode_binary(binary_subtypes subtype, binary_cdata data) {
        encoder_.encode_binary(index(), subtype, data);
        return *this;
      }

      array_encoder& encode_undefined() LIBBASSOON_DEPRECATED {
        encoder_.encode_undefined(index());
        return *this;
      }

      array_encoder& encode_object_id(object_id_cdata id) {
        encoder_.encode_object_id(index(), id);
        return *this;
      }

      array_encoder& encode_boolean(bool value) {
        encoder_.encode_boolean(index(), value);
        return *this;
      }

      array_encoder& encode_utc_datetime(std::int64_t value) {
        encoder_.encode_utc_datetime(index(), value);
        return *this;
      }

      array_encoder& encode_null() {
        encoder_.encode_null(index());
        return *this;
      }

      array_encoder& encode_regex(cstring_cdata regex, cstring_cdata options) {
        encoder_.encode_regex(index(), regex, options);
        return *this;
      }

      array_encoder& encode_db_pointer(string_cdata dbname, object_id_cdata id) LIBBASSOON_DEPRECATED {
        encoder_.encode_db_pointer(index(), dbname, id);
        return *this;
      }

      array_encoder& encode_javascript(string_cdata code) {
        encoder_.encode_javascript(index(), code);
        return *this;
      }

      array_encoder& encode_symbol(string_cdata symbol) {
        encoder_.encode_symbol(index(), symbol);
        return *this;
      }

      array_encoder& encode_scoped_javascript(string_cdata code, void const* scope) {
        encoder_.encode_scoped_javascript(index(), code, scope);
        return *this;
      }

      array_encoder& encode_int32(std::int32_t value) {
        encoder_.encode_int32(index(), value);
        return *this;
      }

      array_encoder& encode_timestamp(std::int64_t value) {
        encoder_.encode_timestamp(index(), value);
        return *this;
      }

      array_encoder& encode_int64(std::int64_t value) {
        encoder_.encode_int64(index(), value);
        return *this;
      }

      array_encoder& encode_min_key() {
        encoder_.encode_min_key(index());
        return *this;
      }

      array_encoder& encode_max_key() {
        encoder_.encode_max_key(index());
        return *this;
      }

      auto start_subdocument() -> decltype(std::declval<encoder_type>().start_subdocument(std::declval<cstring_cdata>())) {
        return encoder_.start_subdocument(index());
      }

      auto start_subarray() -> decltype(std::declval<encoder_type>().start_subdocument(std::declval<cstring_cdata>())) {
        return encoder_.start_subarray(index());
      }

      encoder_type& finish() {
        return encoder_.finish();
      }

    private:
      typename details::encoder_storage<encoder_type>::type encoder_;
    };

    // TODO(acm): Check r-value ref here.
    template<typename encoder_type>
    array_encoder<encoder_type> make_array_encoder(encoder_type&& encoder) {
        return array_encoder<encoder_type>(encoder);
    }

  } // namespace bson
} // namespace bassoon


#endif // included_4c0e4038_bea4_448f_8d89_127de0ccd9c2

#ifndef included_bbc5a9a8_6e97_454f_8f03_5703734857b0
#define included_bbc5a9a8_6e97_454f_8f03_5703734857b0

#include <stack>
#include <vector>

#include <bassoon/abstract_encoder.hpp>

namespace bassoon {
  namespace bson {

    ///
    /// A concrete encoder implements abstract_encoder over a particular writer type
    /// by maintaining a stack of basic encoders.
    ///
    template<typename writer_type>
    class concrete_encoder : public abstract_encoder {
      typedef encoder<writer_type> encoder_type;

    public:

      explicit concrete_encoder(typename encoder_type::writer_type& writer) {
        encoders_.push(encoder_type::start_document(writer));
      }

      concrete_encoder(const concrete_encoder&) = delete;
      concrete_encoder& operator=(const concrete_encoder&) = delete;

      virtual bool ok() const final override {
        return current().ok();
      }

      virtual concrete_encoder& encode_floating_point(cstring_cdata name, double_t value) final override {
        current().encode_floating_point(name, value);
        return *this;
      }

      virtual concrete_encoder& encode_utf8_string(cstring_cdata name, string_cdata value) final override {
        current().encode_utf8_string(name, value);
        return *this;
      }

      virtual concrete_encoder& encode_subdocument(cstring_cdata name, void const* subdocument) final override {
        current().encode_subdocument(name, subdocument);
        return *this;
      }

      virtual concrete_encoder& encode_as_subdocument(cstring_cdata name, binary_cdata data) final override {
        current().encode_as_subdocument(name, data);
        return *this;
      }

      virtual concrete_encoder& encode_subarray(cstring_cdata name, void const* subarray) final override {
        current().encode_subarray(name, subarray);
        return *this;
      }

      virtual concrete_encoder& encode_as_subarray(cstring_cdata name, binary_cdata data) final override {
        current().encode_as_subarray(name, data);
        return *this;
      }

      virtual concrete_encoder& encode_binary(cstring_cdata name, binary_subtypes subtype, binary_cdata data) final override {
        current().encode_binary(name, subtype, data);
        return *this;
      }

      virtual concrete_encoder& encode_undefined(cstring_cdata name) LIBBASSOON_DEPRECATED final override {
        current().encode_undefined(name);
        return *this;
      }

      virtual concrete_encoder& encode_object_id(cstring_cdata name, object_id_cdata id) final override {
        current().encode_object_id(name, id);
        return *this;
      }

      virtual concrete_encoder& encode_boolean(cstring_cdata name, bool value) final override {
        current().encode_boolean(name, value);
        return *this;
      }

      virtual concrete_encoder& encode_utc_datetime(cstring_cdata name, std::int64_t value) final override {
        current().encode_utc_datetime(name, value);
        return *this;
      }

      virtual concrete_encoder& encode_null(cstring_cdata name) final override {
        current().encode_null(name);
        return *this;
      }

      virtual concrete_encoder& encode_regex(cstring_cdata name, cstring_cdata regex, cstring_cdata options) final override {
        current().encode_regex(name, regex, options);
        return *this;
      }

      virtual concrete_encoder& encode_db_pointer(cstring_cdata name, string_cdata dbname, object_id_cdata id) LIBBASSOON_DEPRECATED final override {
        current().encode_db_pointer(name, dbname, id);
        return *this;
      }

      virtual concrete_encoder& encode_javascript(cstring_cdata name, string_cdata code) final override {
        current().encode_javascript(name, code);
        return *this;
      }

      virtual concrete_encoder& encode_symbol(cstring_cdata name, string_cdata symbol) final override {
        current().encode_symbol(name, symbol);
        return *this;
      }

      virtual concrete_encoder& encode_scoped_javascript(cstring_cdata name, string_cdata code, void const* scope) final override {
        current().encode_scoped_javascript(name, code, scope);
        return *this;
      }

      virtual concrete_encoder& encode_int32(cstring_cdata name, std::int32_t value) final override {
        current().encode_int32(name, value);
        return *this;
      }

      virtual concrete_encoder& encode_timestamp(cstring_cdata name, std::int64_t value) final override {
        current().encode_timestamp(name, value);
        return *this;
      }

      virtual concrete_encoder& encode_int64(cstring_cdata name, std::int64_t value) final override {
        current().encode_int64(name, value);
        return *this;
      }

      virtual concrete_encoder& encode_min_key(cstring_cdata name) final override {
        current().encode_min_key(name);
        return *this;
      }

      virtual concrete_encoder& encode_max_key(cstring_cdata name) final override {
        current().encode_max_key(name);
        return *this;
      }

      virtual concrete_encoder& start_subdocument(cstring_cdata name) final override {
        encoders_.push(current().start_subdocument(name));
        return *this;
      }

      virtual concrete_encoder& start_subarray(cstring_cdata name) final override {
        encoders_.push(current().start_subarray(name));
        return *this;
      }

      virtual concrete_encoder& finish() final override {
        current().finish();
        encoders_.pop();
        return *this;
      }

    private:
      encoder_type& current() {
        return encoders_.top();
      }

      const encoder_type& current() const {
        return encoders_.top();
      }

      std::stack<encoder_type, std::vector<encoder_type>> encoders_;
    };

  }  // namespace bson
}  // namespace bassoon


#endif // included_bbc5a9a8_6e97_454f_8f03_5703734857b0

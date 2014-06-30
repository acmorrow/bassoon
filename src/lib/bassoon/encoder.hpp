#ifndef included_b45d4065_193d_46de_a180_e0c5a5763627
#define included_b45d4065_193d_46de_a180_e0c5a5763627

#include <cassert>
#include <type_traits>
#include <stack>
#include <vector>

#include <bassoon/abstract_encoder.hpp>
#include <bassoon/debug.hpp>
#include <bassoon/endian.hpp>

namespace bassoon {
  namespace bson {

    template<typename writer_type>
    class encoder;

    ///
    /// Create a new document encoder, using 'writer' as the
    /// underlying writer. Call this to create a new top level BSON
    /// document. You can get the same effect by calling
    /// encoder::start_document, but this is a little easier since it
    /// inferrs template types for you.
    ///
    template<typename writer_type>
    encoder<writer_type> start_document(writer_type& writer)
      noexcept(noexcept(std::declval<encoder<writer_type>>().start_document(writer))) {
      return encoder<writer_type>::start_document(writer);
    }

    ///
    /// The concrete BSON encoder. This class derives from
    /// encoder_interface, but is final, meaning that if you have the
    /// static type of your writer the compiler is free to
    /// devirtualize the calls. In that case you should also prefer to
    /// use 'start_subdocument' to 'abstract_start_subdocument', and
    /// 'start_subarray' to 'abstract_start_subarray', since this
    /// classes versions require no dynamic allocation.
    ///
    /// The encoder relies on the underyling writer type to abstract
    /// away memory policy. The encoder will continue to write as long
    /// as the underlying writer object states that it is 'ok'. It is
    /// up to the writer implementation to honor this contract.
    ///
    /// See @encoder_interface for documentations on most of the methods
    /// of this class.
    template<typename Writer_type>
    class encoder : public encoder_interface {
    public:
      using writer_type = Writer_type;

    private:
      using cursor_type = typename writer_type::cursor;

      explicit encoder(writer_type& writer) noexcept(noexcept(std::declval<writer_type>().position)) :
        cursor_(writer.position()) {}

      static const length_t k_invalid_length = 0xabababab;

    public:
      static bool const writer_is_noexcept =
        noexcept(std::declval<writer_type>().distance) &&
        noexcept(std::declval<writer_type>().ok)       &&
        noexcept(std::declval<writer_type>().position) &&
        noexcept(std::declval<writer_type>().reserve)  &&
        noexcept(std::declval<writer_type>().write)    &&
        noexcept(std::declval<writer_type>().write_at);

      static bool const cursor_is_noexcept =
        noexcept(std::declval<cursor_type>().writer);

      static bool const is_noexcept =
        writer_is_noexcept &&
        cursor_is_noexcept;

      ///
      /// Constructs a new encoder to build a document. The encoder
      /// will use 'writer' to access memory. While you can call this
      /// directly, the free function bson::start_document is
      /// preferred as it can infer the template type based on the
      /// writer. You should use it when possible.
      ///
      static encoder start_document(writer_type& writer) noexcept(is_noexcept) {
        encoder new_document(writer);
        new_document.private_start_document();
        return new_document;
      }

      // NOTE(acm): Keep this default. It is tempting to add logic
      // here that checks that the sentinel was correctly overwritten,
      // as this would detect the failure to call 'finish' but any
      // body here seems to worsen codegen.
      virtual ~encoder() noexcept final override = default;

      virtual bool ok() const noexcept(noexcept(std::declval<writer_type>().ok)) final override {
        return writer().ok();
      }

      virtual encoder& encode_floating_point(cstring_cdata name, double_t value) noexcept(is_noexcept) final override {
        floating_point_element_encoder().encode(writer(), name, value);
        return *this;
      }

      virtual encoder& encode_utf8_string(cstring_cdata name, string_cdata value) noexcept(is_noexcept) final override {
        utf8_string_element_encoder().encode(writer(), name, value);
        return *this;
      }

      virtual encoder& encode_subdocument(cstring_cdata name, void const* subdocument) noexcept(is_noexcept) final override {
        document_element_encoder().encode(writer(), name, subdocument);
        return *this;
      }

      virtual encoder& encode_as_subdocument(cstring_cdata name, binary_cdata data) noexcept(is_noexcept) final override {
        document_element_encoder().encode(writer(), name, data);
        return *this;
      }

      virtual encoder& encode_subarray(cstring_cdata name, void const* subarray) noexcept(is_noexcept) final override {
        array_element_encoder().encode(writer(), name, subarray);
        return *this;
      }

      virtual encoder& encode_as_subarray(cstring_cdata name, binary_cdata data) noexcept(is_noexcept) final override {
        array_element_encoder().encode(writer(), name, data);
        return *this;
      }

      virtual encoder& encode_binary(cstring_cdata name, binary_subtypes subtype, binary_cdata data) noexcept(is_noexcept) {
        binary_element_encoder().encode(writer(), name, subtype, data);
        return *this;
      }

      virtual encoder& encode_undefined(cstring_cdata name) noexcept(is_noexcept) final override LIBBASSOON_DEPRECATED {
        undefined_element_encoder().encode(writer(), name);
        return *this;
      }

      virtual encoder& encode_object_id(cstring_cdata name, object_id_cdata id) noexcept(is_noexcept) final override {
        object_id_element_encoder().encode(writer(), name, id);
        return *this;
      }

      virtual encoder& encode_boolean(cstring_cdata name, bool value) noexcept(is_noexcept) final override {
        boolean_element_encoder().encode(writer(), name, value);
        return *this;
      }

      virtual encoder& encode_utc_datetime(cstring_cdata name, int64_t value) noexcept(is_noexcept) final override {
        utc_datetime_element_encoder().encode(writer(), name, value);
        return *this;
      }

      virtual encoder& encode_null(cstring_cdata name) noexcept(is_noexcept) final override {
        null_element_encoder().encode(writer(), name);
        return *this;
      }

      virtual encoder& encode_regex(cstring_cdata name, cstring_cdata regex, cstring_cdata options) noexcept(is_noexcept) final override {
        regex_element_encoder().encode(writer(), name, regex, options);
        return *this;
      }

      virtual encoder& encode_db_pointer(cstring_cdata name, string_cdata dbname, object_id_cdata id) noexcept(is_noexcept) final override LIBBASSOON_DEPRECATED {
        db_pointer_element_encoder().encode(writer(), name, dbname, id);
        return *this;
      }

      virtual encoder& encode_javascript(cstring_cdata name, string_cdata code) noexcept(is_noexcept) final override {
        javascript_element_encoder().encode(writer(), name, code);
        return *this;
      }

      virtual encoder& encode_symbol(cstring_cdata name, string_cdata symbol) noexcept(is_noexcept) final override {
        symbol_element_encoder().encode(writer(), name, symbol);
        return *this;
      }

      virtual encoder& encode_scoped_javascript(cstring_cdata name, string_cdata code, void const* scope) noexcept(is_noexcept) final override  {
        scoped_javascript_element_encoder().encode(writer(), name, code, scope);
        return *this;
      }

      virtual encoder& encode_int32(cstring_cdata name, std::int32_t value) noexcept(is_noexcept) final override {
        int32_element_encoder().encode(writer(), name, value);
        return *this;
      }

      virtual encoder& encode_timestamp(cstring_cdata name, std::int64_t value) noexcept(is_noexcept) final override {
        timestamp_element_encoder().encode(writer(), name, value);
        return *this;
      }

      virtual encoder& encode_int64(cstring_cdata name, std::int64_t value) noexcept(is_noexcept) final override {
        int64_element_encoder().encode(writer(), name, value);
        return *this;
      }

      virtual encoder& encode_min_key(cstring_cdata name) noexcept(is_noexcept) final override {
        min_element_encoder().encode(writer(), name);
        return *this;
      }

      virtual encoder& encode_max_key(cstring_cdata name) noexcept(is_noexcept) final override {
        max_element_encoder().encode(writer(), name);
        return *this;
      }

      ///
      /// Start a new subdocument named 'name'. You must call 'finish' on the returned encoder
      /// before using this encoder.
      ///
      encoder start_subdocument(cstring_cdata name) noexcept(is_noexcept) {
        private_start_subdocument(name);
        encoder new_document(writer());
        new_document.private_start_document();
        return new_document;
      }

      ///
      /// Start a new subarray named 'name'. You must call 'finish' on the returned encoder
      /// before using this encoder.
      ///
      encoder start_subarray(cstring_cdata name) noexcept(is_noexcept) {
        private_start_subarray(name);
        encoder new_document(writer());
        new_document.private_start_document();
        return new_document;
      }

      ///
      /// Finish object or array that we are encoding.
      ///
      encoder& finish() noexcept(is_noexcept) {
        wrapped_writer().
          template encode_with<null_byte_encoder>();
        length_t const written = writer().distance(cursor_, writer().position());

        if (k_debug) {
          // Check that the memory location where we are going to
          // write the final length still contains the sentinel value
          // that we encoded in document_start_encoder. Otherwise,
          // someone overwrote our memory. If you are getting
          // assertion failures here you have probably started more
          // than one concurrent subdocument this objects parent.
          assert(k_invalid_length == read_length_from_cursor());
        }

        writer().write_at(cursor_, &written, sizeof(written));
        return *this;
      }

    private:

      // TODO(acm): Currently the primitive encoders are private. If
      // we made them protected and documented them, then it would be
      // possible to build custom subclasses of encoder that could
      // re-use them. They would need to be documented, and we would
      // need a use-case for opening up the encoder to extension.

      struct raw_data_encoder {
        template<typename data_type>
        static void encode(writer_type& writer, data_type data) noexcept(is_noexcept) {
          if (writer.reserve(data.size))
            writer.write(data.data, data.size);
        }
      };

      template<typename value_type>
      struct value_encoder {

        // NOTE: This should say is_trivially_copyable, but libstdc++ doesn't have that yet.
        static_assert(std::is_trivial<value_type>::value,
                      "value_type must be trivially copyable");

        static void encode(writer_type& writer, value_type value) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<raw_data_encoder>(binary_cdata(&value, sizeof(value)));
        }
      };

      using byte_encoder = value_encoder<byte_t>;

      template<byte_t byte>
      struct fixed_byte_encoder {
        static void encode(writer_type& writer) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<byte_encoder>(byte);
        }
      };

      using type_encoder = value_encoder<types>;

      template<types type>
      struct fixed_type_encoder {
        static void encode(writer_type& writer) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<type_encoder>(type);
        }
      };

      template<typename integral_type>
      struct little_endian_integer_encoder {

        static_assert(std::is_integral<integral_type>::value,
                      "integral_type must be integral");

        static void encode(writer_type& writer, integral_type value) noexcept(is_noexcept) {
          auto converted = endian::native_to_little(value);
          wrap(writer)
            .template encode_with<value_encoder<integral_type>>(converted);
        }
      };

      struct cstring_encoder {
        static void encode(writer_type& writer, cstring_cdata data) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<raw_data_encoder>(data);
        }
      };

      using element_name_encoder = cstring_encoder;

      struct type_and_name_encoder {
        static void encode(writer_type& writer, types type, cstring_cdata name) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<type_encoder>(type)
            .template encode_with<element_name_encoder>(name);
        }
      };

      template<types type>
      struct fixed_type_and_name_encoder {
        static void encode(writer_type& writer, cstring_cdata name) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<type_and_name_encoder>(type, name);
        }
      };

      using int32_t_encoder = little_endian_integer_encoder<std::int32_t>;
      using int64_t_encoder = little_endian_integer_encoder<std::int64_t>;
      using length_t_encoder = little_endian_integer_encoder<length_t>;

      // TODO(acm): endian? IEEE 754?
      using double_t_encoder = value_encoder<double_t>;

      struct boolean_encoder {
        static void encode(writer_type& writer, bool value) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<byte_encoder>(
              value ?
              static_cast<byte_t>(values::true_) :
              static_cast<byte_t>(values::false_));
        }
      };

      using binary_subtype_encoder = value_encoder<binary_subtypes>;

      struct binary_encoder {
        static void encode(writer_type& writer, binary_subtypes subtype, binary_cdata data) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<length_t_encoder>(data.size)
            .template encode_with<binary_subtype_encoder>(subtype)
            .template encode_with<raw_data_encoder>(data);
        }
      };

      struct regex_encoder {
        static void encode(writer_type& writer, cstring_cdata const& regex, cstring_cdata const& options) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<cstring_encoder>(regex)
            .template encode_with<cstring_encoder>(options);
        }
      };

      struct object_id_encoder {
        static void encode(writer_type& writer, object_id_cdata id) noexcept(is_noexcept) {
          wrap(writer).
            template encode_with<raw_data_encoder>(id);
        }
      };

      struct string_encoder {
        static void encode(writer_type& writer, string_cdata data) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<length_t_encoder>(data.size)
            .template encode_with<cstring_encoder>(
              cstring_cdata(data.data, data.size, string_data_details::null_included_tag()));
        }
      };

      struct db_pointer_encoder {
        static void encode(writer_type& writer, string_cdata dbname, object_id_cdata id) noexcept(is_noexcept)  {
          wrap(writer)
            .template encode_with<string_encoder>(dbname)
            .template encode_with<object_id_encoder>(id);
        }
      };

      using null_byte_encoder = fixed_byte_encoder<static_cast<byte_t>(values::null)>;

      struct raw_document_encoder {
        static void encode(writer_type& writer, void const* document) noexcept(is_noexcept) {

          // Read length out of the provided document.
          length_t length = read_length_from_document(document);

          // TODO(acm): CHECK ME!
          length += sizeof(length) + 1;  // account for length and \0 byte.
          // Write the data
          wrap(writer).template encode_with<raw_data_encoder>(binary_cdata(document, length));
        }

        static void encode(writer_type& writer, binary_cdata data) noexcept(is_noexcept) {
          // TODO(acm): CHECK ME!
          length_t const encoded_length = sizeof(data.size) + data.size + 1;
          wrap(writer)
            .template encode_with<length_t_encoder>(encoded_length)
            .template encode_with<raw_data_encoder>(data)
            .template encode_with<null_byte_encoder>();
        }
      };

      struct scoped_javascript_encoder {
        static void encode(writer_type& writer, string_cdata code, void const* scope) noexcept(is_noexcept) {
          // TODO(acm): DRY this with raw_document_encoder above. I
          // don't like the way either of these has to pre-compute the
          // length, but I'm not sure how else to do it.

          // Read length out of the provided scope document.
          length_t length = read_length_from_document(scope);

          // TODO(acm): CHECK ME!
          length += sizeof(length) + 1;  // account for length and \0 byte.
          length += sizeof(length) + code.size;  // account for length of code.

          wrap(writer)
            .template encode_with<length_t_encoder>(length)
            .template encode_with<string_encoder>(code)
            .template encode_with<raw_document_encoder>(scope);
        }
      };

      template<types type, typename value_encoder>
      struct element_encoder {
        template<typename ...ArgTypes>
        static void encode(writer_type& writer, cstring_cdata name, ArgTypes&& ...args) noexcept(is_noexcept) {
          wrap(writer)
            .template encode_with<fixed_type_and_name_encoder<type>>(name)
            .template encode_with<value_encoder>(std::forward<ArgTypes>(args)...);
        }
      };

      struct document_start_encoder {
        static void encode(writer_type& writer) noexcept(is_noexcept) {
          // We use a sentintel value that marks the length as
          // 'allocated but uninitialized'. In debug builds, we double
          // check that we still have the right pattern when 'finish'
          // is called.
          wrap(writer)
            .template encode_with<length_t_encoder>(k_invalid_length);
        }
      };

      using floating_point_element_encoder =
        element_encoder<types::floating_point, double_t_encoder>;

      using utf8_string_element_encoder =
        element_encoder<types::utf8_string, string_encoder>;

      using document_element_encoder =
        element_encoder<types::document, raw_document_encoder>;

      using array_element_encoder =
        element_encoder<types::array, raw_document_encoder>;

      using binary_element_encoder =
        element_encoder<types::binary, binary_encoder>;

      using undefined_element_encoder =
        fixed_type_and_name_encoder<types::undefined_no_deprecated>;

      using object_id_element_encoder =
        element_encoder<types::object_id, object_id_encoder>;

      using boolean_element_encoder =
        element_encoder<types::boolean, boolean_encoder>;

      using utc_datetime_element_encoder =
        element_encoder<types::utc_datetime, int64_t_encoder>;

      using null_element_encoder =
        fixed_type_and_name_encoder<types::null>;

      using regex_element_encoder =
        element_encoder<types::regex, regex_encoder>;

      using db_pointer_element_encoder =
        element_encoder<types::db_pointer_no_deprecated, db_pointer_encoder>;

      using javascript_element_encoder =
        element_encoder<types::javascript, string_encoder>;

      using symbol_element_encoder =
        element_encoder<types::symbol, string_encoder>;

      using scoped_javascript_element_encoder =
        element_encoder<types::scoped_javascript, scoped_javascript_encoder>;

      using int32_element_encoder =
        element_encoder<types::int32, int32_t_encoder>;

      using timestamp_element_encoder =
        element_encoder<types::timestamp, int64_t_encoder>;

      using int64_element_encoder =
        element_encoder<types::int64, int64_t_encoder>;

      using min_element_encoder =
        fixed_type_and_name_encoder<types::min>;

      using max_element_encoder =
        fixed_type_and_name_encoder<types::max>;

    private:

      virtual void private_start_document() noexcept(is_noexcept) {
        wrapped_writer()
          .template encode_with<document_start_encoder>();
      }

      virtual void private_start_subdocument(cstring_cdata name) noexcept(is_noexcept) {
        wrapped_writer()
          .template encode_with<fixed_type_and_name_encoder<types::document>>(name);
      }

      virtual void private_start_subarray(cstring_cdata name) noexcept(is_noexcept) {
        wrapped_writer()
          .template encode_with<fixed_type_and_name_encoder<types::array>>(name);
      }

    private:
      writer_type& writer() const {
        return cursor_.writer();
      }

      struct writer_wrapper {
        writer_wrapper(writer_type& writer) noexcept :
          writer_(writer) {}

        template<typename T, typename ...ArgTypes>
        writer_wrapper& encode_with(ArgTypes&&... args) {
          if (writer_.ok())
            T::encode(writer_, std::forward<ArgTypes>(args)...);
          return *this;
        }

        writer_type& writer_;
      };

      static writer_wrapper wrap(writer_type& writer) {
        return writer_wrapper(writer);
      }

      writer_wrapper wrapped_writer() noexcept {
        return wrap(writer());
      }

      static length_t read_length_from_address(void const* address) noexcept {
        length_t length;
        std::memcpy(&length, address, sizeof(length));
        return length;
      }

      // Interpret the bytes pointed to by the cursor as a length_t.
      length_t read_length_from_cursor() const noexcept {
        return read_length_from_address(cursor_.address());
      }

      static length_t read_length_from_document(void const* document) {
        return endian::little_to_native(read_length_from_address(document));
      }

      // NOTE(acm): This is the only state in the encoder. It stores a
      // cursor that captures where we will write the total document
      // length when 'finish' is called. The cursor also holds a
      // reference to its writer so that we can access the writer
      // easily.
      const cursor_type cursor_;
    };

    template<typename writer_type>
    const length_t encoder<writer_type>::k_invalid_length;


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

#endif // included_b45d4065_193d_46de_a180_e0c5a5763627

#ifndef included_b45d4065_193d_46de_a180_e0c5a5763627
#define included_b45d4065_193d_46de_a180_e0c5a5763627

#include <cassert>
#include <type_traits>

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
    /// abstract_encoder, but is final, meaning that if you have the
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
    /// See @abstract_encoder for documentations on most of the methods
    /// of this class.
    template<typename Writer_type>
    class encoder : public abstract_encoder {

      using writer_type = Writer_type;
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

      virtual void encode_floating_point(cstring_cdata name, double_t value) noexcept(is_noexcept) final override {
        floating_point_element_encoder().encode(writer(), name, value);
      }

      virtual void encode_utf8_string(cstring_cdata name, string_cdata value) noexcept(is_noexcept) final override {
        utf8_string_element_encoder().encode(writer(), name, value);
      }

      virtual void encode_subdocument(cstring_cdata name, void const* subdocument) noexcept(is_noexcept) final override {
        document_element_encoder().encode(writer(), name, subdocument);
      }

      virtual void encode_as_subdocument(cstring_cdata name, binary_cdata data) noexcept(is_noexcept) final override {
        document_element_encoder().encode(writer(), name, data);
      }

      ///
      /// The non-virtual analogue of
      /// abstract_encoder::abstract_start_subdocument. This version
      /// performs no dynamic allocation, so prefer to use this when
      /// you have the encoder as a concrete type. Operations on the
      /// returned encoder will build the subdocument. There must be
      /// only one subencoder active for this encoder at any
      /// time. While a subencoder is active, all higher level
      /// encoders must not be accessed. You must call 'finish' on the
      /// returned encoder to complete the subdocument.
      ///
      encoder start_subdocument(cstring_cdata name) noexcept(is_noexcept) {
        private_start_subdocument(name);
        encoder new_document(writer());
        new_document.private_start_document();
        return new_document;
      }

      virtual void encode_subarray(cstring_cdata name, void const* subarray) noexcept(is_noexcept) final override {
        array_element_encoder().encode(writer(), name, subarray);
      }

      virtual void encode_as_subarray(cstring_cdata name, binary_cdata data) noexcept(is_noexcept) final override {
        array_element_encoder().encode(writer(), name, data);
      }

      ///
      /// The non-virtual analogue of
      /// abstract_encoder::abstract_start_array. This version
      /// performs no dynamic allocation, so prefer to use this when
      /// you have the encoder as a concrete type. Operations on the
      /// returned encoder will build the subdocument. There must be
      /// only one subencoder active for this encoder at any
      /// time. While a subencoder is active, all higher level
      /// encoders must not be accessed. You must call 'finish' on the
      /// returned encoder to complete the subdocument.
      ///
      encoder start_subarray(cstring_cdata name) noexcept(is_noexcept) {
        private_start_subarray(name);
        encoder new_document(writer());
        new_document.private_start_document();
        return new_document;
      }

      virtual void encode_binary(cstring_cdata name, binary_subtypes subtype, binary_cdata data) noexcept(is_noexcept) {
        binary_element_encoder().encode(writer(), name, subtype, data);
      }

      virtual void encode_undefined(cstring_cdata name) noexcept(is_noexcept) final override BASSOON_DEPRECATED_API {
        undefined_element_encoder().encode(writer(), name);
      }

      virtual void encode_object_id(cstring_cdata name, object_id_cdata id) noexcept(is_noexcept) final override {
        object_id_element_encoder().encode(writer(), name, id);
      }

      virtual void encode_boolean(cstring_cdata name, bool value) noexcept(is_noexcept) final override {
        boolean_element_encoder().encode(writer(), name, value);
      }

      virtual void encode_utc_datetime(cstring_cdata name, int64_t value) noexcept(is_noexcept) final override {
        utc_datetime_element_encoder().encode(writer(), name, value);
      }

      virtual void encode_null(cstring_cdata name) noexcept(is_noexcept) final override {
        null_element_encoder().encode(writer(), name);
      }

      virtual void encode_regex(cstring_cdata name, cstring_cdata regex, cstring_cdata options) noexcept(is_noexcept) final override {
        regex_element_encoder().encode(writer(), name, regex, options);
      }

      virtual void encode_db_pointer(cstring_cdata name, string_cdata dbname, object_id_cdata id) noexcept(is_noexcept) final override BASSOON_DEPRECATED_API {
        db_pointer_element_encoder().encode(writer(), name, dbname, id);
      }

      virtual void encode_javascript(cstring_cdata name, string_cdata code) noexcept(is_noexcept) final override {
        javascript_element_encoder().encode(writer(), name, code);
      }

      virtual void encode_symbol(cstring_cdata name, string_cdata symbol) noexcept(is_noexcept) final override {
        symbol_element_encoder().encode(writer(), name, symbol);
      }

      virtual void encode_scoped_javascript(cstring_cdata name, string_cdata code, void const* scope) noexcept(is_noexcept) final override  {
        scoped_javascript_element_encoder().encode(writer(), name, code, scope);
      }

      virtual void encode_int32(cstring_cdata name, std::int32_t value) noexcept(is_noexcept) final override {
        int32_element_encoder().encode(writer(), name, value);
      }

      virtual void encode_timestamp(cstring_cdata name, std::int64_t value) noexcept(is_noexcept) final override {
        timestamp_element_encoder().encode(writer(), name, value);
      }

      virtual void encode_int64(cstring_cdata name, std::int64_t value) noexcept(is_noexcept) final override {
        int64_element_encoder().encode(writer(), name, value);
      }

      virtual void encode_min_key(cstring_cdata name) noexcept(is_noexcept) final override {
        min_element_encoder().encode(writer(), name);
      }

      virtual void encode_max_key(cstring_cdata name) noexcept(is_noexcept) final override {
        max_element_encoder().encode(writer(), name);
      }

      virtual void finish() noexcept(is_noexcept) final override {
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

        static_assert(std::is_trivially_copyable<value_type>::value,
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
          auto converted = host_to_little_endian(value);
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

      // NOTE(acm): These are the hooks that let us work with an
      // encoder via the abstract_encoder interface.
      virtual std::unique_ptr<abstract_encoder> clone() const final override {
        return std::unique_ptr<encoder>(new encoder(writer()));
      }

      virtual void private_start_document() noexcept(is_noexcept) final override {
        wrapped_writer()
          .template encode_with<document_start_encoder>();
      }

      virtual void private_start_subdocument(cstring_cdata name) noexcept(is_noexcept) final override {
        wrapped_writer()
          .template encode_with<fixed_type_and_name_encoder<types::document>>(name);
      }

      virtual void private_start_subarray(cstring_cdata name) noexcept(is_noexcept) final override {
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
        return little_endian_to_host(read_length_from_address(document));
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

  }  // namespace bson
}  // namespace bassoon

#endif // included_b45d4065_193d_46de_a180_e0c5a5763627

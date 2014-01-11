#ifndef included_c68a1e82_cccc_4cfe_af5e_cbb28bb63076
#define included_c68a1e82_cccc_4cfe_af5e_cbb28bb63076

namespace bassoon {
  namespace bson {

    template<typename reader_type, typename handler_type>
    class decoder;

    // TODO(acm): Move to own header
    class abstract_decoder {
    public:
      virtual ~abstract_decoder() = default;
    };

    template<typename Reader_type, typename Handler_type>
    class decoder : public abstract_decoder {

    public:
      using reader_type = Reader_type;
      using cursor_type = typename reader_type::cursor;
      using handler_type = Handler_type;

      decoder(handler_type& handler) noexcept
        : handler_(handler) {}

      void decode_document(cursor_type cursor) {
        length_t const length = read_length_from_cursor(cursor);
        cursor.advance(sizeof(length_t));

        cursor end = cursor;
        end.advance(length - sizeof(length_t) - 1);
        const cursor cend = end;

        while (cursor != end) {
          type const current_type = read_type_from_cursor(cursor);
          cursor.advance(sizeof(type));
          // we are now pointing to a name
          cstring_cdata const name_data(cursor());
          cursor.advance(name_data.size);

          switch(current_type) {
            case floating_point:
              handler_result = handle_floating_point(name_data, cursor);
              break;
            case utf8_string:
              handler_result = handle_utf8_string(name_data, cursor);
              break;
            case document:
              handler_result = handle_document(name_data, cursor);
              break;
            case array:
              handler_result = handle_array(name_data, cursor);
              break;
            case binary:
              handler_result = handle_binary(name_data, cursor);
              break;
            case undefined_no_deprecated:
              handler_result = handle_undefined(name_data, cursor);
              break;
            case object_id:
              handler_result = handle_objec_id(name_data, cursor);
              break;
            case boolean:
              handler_result = handle_boolean(name_data, cursor);
              break;
            case utc_datetime:
              handler_result = handle_utc_datetime(name_data, cursor);
              break;
            case null:
              handler_result = handle_null(name_data, cursor);
              break;
            case regex:
              handler_result = handle_regex(name_data, cursor);
              break;
            case db_pointer_no_deprecated:
              handler_result = handle_db_pointer(name_data, cursor);
              break;
            case javascript:
              handler_result = handle_javascript(name_data, cursor);
              break;
            case symbol:
              handler_result = handle_symbol(name_data, cursor);
              break;
            case scoped_javascript:
              handler_result = handle_scoped_javascript(name_data, cursor);
              break;
            case int32:
              handler_result = handle_int32(name_data, cursor);
              break;
            case timestamp:
              handler_result = handle_timestamp(name_data, cursor);
              break;
            case int64:
              handler_result = handle_int64(name_data, cursor);
              break;
            case min:
              handler_result = handle_min(name_data, cursor);
              break;
            case max:
              handler_result = handle_max(name_data, cursor);
              break;
            default:
              handler_result = handle_unknown_type(name_data, cursor);
              // TOOD(acm): Unknown field type!
          }

          switch(handler_result) {
          }

        }
      }

    private:

      handler_result handle_floating_point(cstring_cdata const& name_data, cursor_type& cursor) {
        double_t value;
        std::memcpy(&value, cursor.address(), sizeof(value));
        return handler_.on_floating_point(name_data, value);
      }

      handler_result handle_utf8_string(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_document(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_array(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_binary(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_undefined(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_object_id(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_boolean(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_utc_datetime(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_null(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_regex(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_db_pointer(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_javascript(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_symbol(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_scoped_javascript(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_int32(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_timestamp(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_int64(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_min(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_max(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      handler_result handle_unknown_type(cstring_cdata const& name_data, cursor_type& cursor) {
      }

      reader_type& reader() const{
        return cursor_.reader();
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

      const handler_type& handler_;
    };


  }  // namespace bson
}  // namespace bassoon

#endif // included_c68a1e82_cccc_4cfe_af5e_cbb28bb63076

#ifndef included_c68a1e82_cccc_4cfe_af5e_cbb28bb63076
#define included_c68a1e82_cccc_4cfe_af5e_cbb28bb63076
#pragma once

namespace bassoon {
  namespace bson {

    template<typename reader_type>
    class decoder;

    template<typename Reader_type, typename Handler_type>
    class decoder final : public abstract_decoder {

    public:
      using reader_type = Reader_type;
      using cursor_type = reader_type::cursor;
      using handler_type = Handler_type;

      decoder(handler_type& handler) noexcept(noexcept(reader::position))
        : handler_(handler) {}

      void decode(cursor_type& cursor) {
        length_t const length
      }

    private:
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

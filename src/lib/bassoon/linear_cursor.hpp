#ifndef included_e27ebc5f_14ef_46ca_b867_06587de27fd3
#define included_e27ebc5f_14ef_46ca_b867_06587de27fd3

#include <iterator>

namespace bassoon {
  namespace bson {

    template<typename Writer_type>
    class linear_cursor {
    public:
      using writer_type = Writer_type;
      using base_cursor_type = typename writer_type::base_cursor_type;

      linear_cursor(writer_type& writer, base_cursor_type base_cursor)
        : writer_(writer)
        , base_cursor_(base_cursor) {}

      writer_type& writer() noexcept {
        return writer_;
      }

      writer_type& writer() const noexcept {
        return writer_;
      }

      typename writer_type::base_cursor_type& address() noexcept {
        return base_cursor_;
      }

      typename writer_type::base_cursor_type const& address() const noexcept {
        return base_cursor_;
      }

      void advance(std::size_t size) noexcept {
        std::advance(base_cursor_, size);
      }

    private:
      writer_type& writer_;
      base_cursor_type base_cursor_;
    };

  } // namespace bson
} // namespace bassoon

#endif // included_e27ebc5f_14ef_46ca_b867_06587de27fd3

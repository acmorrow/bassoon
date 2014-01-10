#ifndef included_848339ba_80e8_4f7c_827c_35433d656691
#define included_848339ba_80e8_4f7c_827c_35433d656691

#include <array>

#include <bassoon/linear_cursor.hpp>

namespace bassoon {
  namespace bson {

    template<class T, std::size_t N>
    class array_writer {

      typedef std::array<T, N> buffer_type;

    public:
      using base_cursor_type = typename buffer_type::iterator;
      using cursor = linear_cursor<array_writer>;

      array_writer(buffer_type& buffer) noexcept
        : buffer_(buffer)
        , position_(*this, buffer_.begin())
        , ok_(true) {}

      bool reserve(std::size_t size) noexcept {
        ok_ = (available() >= size);
        return ok();
      }

      cursor position() const noexcept {
        return position_;
      }

      void advance(size_t size) noexcept {
        assert(size <= available());
        position_.advance(size);
      }

      bool ok() const noexcept {
        return ok_;
      }

      cursor begin() noexcept {
        return cursor(*this, buffer_.begin());
      }

      cursor end() noexcept {
        return cursor(*this, buffer_.end());
      }

      std::size_t valid() noexcept {
        return distance(begin(), position());
      }

      std::size_t distance(const cursor& a, const cursor& b) noexcept {
        return std::distance(a.address(), b.address());
      }

      void write(void const* data, std::size_t size) noexcept {
        write_at(position(), data, size);
        advance(size);
      }

      void write_at(cursor cursor, void const* data, std::size_t size) noexcept {
        write_private(cursor.address(), data, size);
      }

    private:
      void write_private(void* address, void const* data, std::size_t size) noexcept {
        std::memcpy(address, data, size);
      }

      std::size_t available() noexcept {
        return distance(position(), end());
      }

      buffer_type& buffer_;
      cursor position_;
      bool ok_;
    };

    template<typename T, std::size_t N>
    array_writer<T, N> make_array_writer(std::array<T, N>& array) {
      return array_writer<T, N>(array);
    }

  }  // namespace bson
}  // namespace bassoon

#endif // included_848339ba_80e8_4f7c_827c_35433d656691

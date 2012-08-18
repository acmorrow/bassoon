#include <array>
#include <iterator>
#include <sstream>
#include <cassert>
#include <iostream>
#include <iomanip>

#include <bassoon/encoder.hpp>
#include <bassoon/decoder.hpp>

namespace {

  using namespace bassoon::bson;

  void dump_bytes(std::ostream& stream, void const* data, std::size_t length) __attribute__((noinline));
  void dump_bytes(std::ostream& stream, void const* data, std::size_t length) {
    std::cout << std::string(reinterpret_cast<char const*>(data), length);
  }

  template<typename W>
  class linear_cursor {
  public:
    using writer_type = W;
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
      // ok_ = (available() >= size);
      return ok();
    }

    cursor position() const noexcept {
      return position_;
    }

    void advance(size_t size) noexcept {
      //assert(size <= available());
      position_.advance(size);
    }

    bool ok() const noexcept {
      return true; //ok_;
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

  template<typename T>
  void make_writer();

  template<typename T, std::size_t N>
  array_writer<T, N> make_writer(std::array<T, N>& array) {
    return array_writer<T, N>(array);
  }

  bool do_example(std::ostream& stream) __attribute__((noinline));
  bool do_example(std::ostream& stream) {
    // { "hello" : "world" }
    std::array<char, 128> buffer;
    auto writer = make_writer(buffer);
    auto document = start_document(writer);
    document.encode_utf8_string("hello", "world");
    document.finish();

    if (document.ok())
      dump_bytes(stream, buffer.begin(), writer.valid());

    return document.ok();
  }

  bool do_nested_example(std::ostream& stream) __attribute__((noinline));
  bool do_nested_example(std::ostream& stream) {
    // { "message" : { "hello" : "world" } }
    std::array<char, 128> buffer;
    auto writer = make_writer(buffer);
    auto document = start_document(writer);
    auto message = document.start_subdocument("message");
    message.encode_utf8_string("hello", "world");
    message.finish();
    document.finish();

    if (document.ok())
      dump_bytes(stream, buffer.begin(), writer.valid());

    return document.ok();
  }

  bool do_nested_abstract(std::ostream& stream) __attribute__((noinline));
  bool do_nested_abstract(std::ostream& stream) {
    // { "message" : { "hello" : "world" } }
    std::array<char, 128> buffer;
    auto writer = make_writer(buffer);
    auto document = start_document(writer);
    auto message = document.abstract_start_subdocument("message");
    message->encode_utf8_string("hello", "world");
    message->finish();
    document.finish();

    if (document.ok())
      dump_bytes(stream, buffer.begin(), writer.valid());

    return document.ok();
  }

  bool do_array(std::ostream& stream) __attribute__((noinline));
  bool do_array(std::ostream& stream) {
    // { "array" : [ "hello", "world" ] }
    std::array<char, 128> buffer;
    auto writer = make_writer(buffer);
    auto document = start_document(writer);
    auto array = document.start_subarray("array");
    array.encode_utf8_string("0", "hello");
    array.encode_utf8_string("1", "world");
    array.finish();
    document.finish();

    if (document.ok())
      dump_bytes(stream, buffer.begin(), writer.valid());

    return document.ok();
  }

  void do_decode() {


  }

} // namespace

int main(int argc, char* argv[]) {
  do_example(std::cout);
  do_nested_example(std::cout);
  do_nested_abstract(std::cout);
  do_array(std::cout);
}

#include <bassoon/encoder.hpp>

#include <array>
#include <gtest/gtest.h>

namespace {

  using namespace bassoon::bson;


  void dump_bytes(std::ostream& stream, void const* data, std::size_t length) {
    static const char items[] = { '0', '1', '2', '3',
                                  '4', '5', '6', '7',
                                  '8', '9', 'a', 'b',
                                  'c', 'd', 'e', 'f' };

    byte_t const* cdata = reinterpret_cast<byte_t const*>(data);
    for (std::size_t i = 0; i != length; ++i) {
      stream << "\\x" << items[(cdata[i] & 0xf0) >> 4] << items[(cdata[i] & 0x0f)];
    }
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

  template<typename T>
  void make_writer();

  template<typename T, std::size_t N>
  array_writer<T, N> make_writer(std::array<T, N>& array) {
    return array_writer<T, N>(array);
  }

  // Check that we can reproduce the example from bsonspec.org
  TEST(BSONSpecDotOrgExamplesTest, EncodeHelloWorld) {

    const char expected[] = {
      0x16, 0x00, 0x00, 0x00,
       0x02, 'h', 'e', 'l', 'l', 'o', 0x00,
      0x06, 0x00, 0x00, 0x00,  'w', 'o', 'r', 'l', 'd', 0x00,
      0x00,
    };

    std::array<char, sizeof(expected)> buffer;

    auto writer = make_writer(buffer);
    EXPECT_TRUE(writer.ok());
    EXPECT_EQ(0, writer.valid());

    auto document = start_document(writer);
    EXPECT_TRUE(writer.ok());

    document.encode_utf8_string("hello", "world");
    EXPECT_TRUE(writer.ok());

    document.finish();
    EXPECT_TRUE(writer.ok());

    EXPECT_EQ(sizeof(expected), writer.valid());
    EXPECT_EQ(0, std::memcmp(&expected[0], writer.begin().address(), sizeof(expected)));
  }

  TEST(BSONSpecDotOrgExamplesTest, EncodeBSONIsAwesome) {

    const byte_t expected[] = {
      0x31, 0x00, 0x00, 0x00,
        0x04, 'B', 'S', 'O', 'N', 0x00,
          0x26, 0x00, 0x00, 0x00,
            0x02, 0x30, 0x00,
              0x08, 0x00, 0x00, 0x00, 'a', 'w', 'e', 's', 'o', 'm', 'e', 0x00,
            0x01, 0x31, 0x00,
              0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x14, 0x40,
            0x10, 0x32, 0x00,
              0xc2, 0x07, 0x00, 0x00,
        0x00,
      0x00,
    };

    std::array<byte_t, sizeof(expected)> buffer;
    memset(&buffer[0], 0, buffer.size());

    auto writer = make_writer(buffer);
    EXPECT_TRUE(writer.ok());
    EXPECT_EQ(0, writer.valid());

    auto document = start_document(writer);
    EXPECT_TRUE(writer.ok());

    auto array = document.start_subarray("BSON");
    EXPECT_TRUE(writer.ok());

    array.encode_utf8_string("0", "awesome");
    EXPECT_TRUE(writer.ok());

    array.encode_floating_point("1", 5.05);
    EXPECT_TRUE(writer.ok());

    array.encode_int32("2", 1986);
    EXPECT_TRUE(writer.ok());

    array.finish();
    EXPECT_TRUE(writer.ok());

    document.finish();
    EXPECT_TRUE(writer.ok());

    EXPECT_EQ(sizeof(expected), writer.valid());
    EXPECT_EQ(0, std::memcmp(&expected[0], writer.begin().address(), sizeof(expected)));
  }

}

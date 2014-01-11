#include <gtest/gtest.h>

#include <bassoon/debug.hpp>
#include <bassoon/encoder.hpp>
#include <bassoon/array_writer.hpp>

namespace {

  using namespace bassoon::bson;

  // Check that we can reproduce the example from bsonspec.org
  TEST(BSONSpecDotOrgExamplesTest, EncodeHelloWorld) {

    const char expected[] = {
      0x16, 0x00, 0x00, 0x00,
       0x02, 'h', 'e', 'l', 'l', 'o', 0x00,
      0x06, 0x00, 0x00, 0x00,  'w', 'o', 'r', 'l', 'd', 0x00,
      0x00,
    };

    std::array<char, sizeof(expected)> buffer;

    auto writer = make_array_writer(buffer);
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

    auto writer = make_array_writer(buffer);
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

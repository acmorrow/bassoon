#include <array>
#include <iterator>
#include <sstream>
#include <cassert>
#include <iostream>
#include <iomanip>

#include <bassoon/array_encoder.hpp>
#include <bassoon/array_writer.hpp>
#include <bassoon/debug.hpp>
#include <bassoon/encoder.hpp>

namespace {

  using namespace bassoon::bson;

  bool do_example(std::ostream& stream) __attribute__((noinline));
  bool do_example(std::ostream& stream) {
    // { "hello" : "world" }
    std::array<char, 128> buffer;
    auto writer = make_array_writer(buffer);
    auto document = start_document(writer);
    document.encode_utf8_string("hello", "world");
    document.finish();

    dump_bytes_as_hex(stream, buffer.begin(), writer.valid());
    std::cout << "\n";

    return document.ok();
  }

  bool do_nested_example(std::ostream& stream) __attribute__((noinline));
  bool do_nested_example(std::ostream& stream) {
    // { "message" : { "hello" : "world" } }
    std::array<char, 128> buffer;
    auto writer = make_array_writer(buffer);
    auto document = start_document(writer);
    auto message = document.start_subdocument("message");
    message.encode_utf8_string("hello", "world");
    message.finish();
    document.finish();

    dump_bytes_as_hex(stream, buffer.begin(), writer.valid());
    std::cout << "\n";

    return document.ok();
  }

  bool do_nested_concrete(std::ostream& stream) __attribute__((noinline));
  bool do_nested_concrete(std::ostream& stream) {
    // { "message" : { "hello" : "world" } }
    std::array<char, 128> buffer;
    auto writer = make_array_writer(buffer);

    const bool ok = concrete_encoder<decltype(writer)>(writer)
        .start_subdocument("message")
        .encode_utf8_string("hello", "world")
        .finish()
        .finish()
        .ok();

    dump_bytes_as_hex(stream, buffer.begin(), writer.valid());
    std::cout << "\n";

    return ok;
  }

  bool do_array_manually(std::ostream& stream) __attribute__((noinline));
  bool do_array_manually(std::ostream& stream) {
    // { "array" : [ "hello", "world" ] }
    std::array<char, 128> buffer;
    auto writer = make_array_writer(buffer);
    auto document = start_document(writer);
    auto array = document.start_subarray("array");
    array.encode_utf8_string("0", "hello");
    array.encode_utf8_string("1", "world");
    array.finish();
    document.finish();

    dump_bytes_as_hex(stream, buffer.begin(), writer.valid());
    std::cout << "\n";

    return document.ok();
  }

  bool do_array(std::ostream& stream) __attribute__((noinline));
  bool do_array(std::ostream& stream) {
    // { "array" : [ "hello", "world" ] }
    std::array<char, 128> buffer;
    auto writer = make_array_writer(buffer);
    auto document = start_document(writer);
    make_array_encoder(document.start_subarray("array"))
      .encode_utf8_string("hello")
      .encode_utf8_string("world")
      .finish();
    document.finish();

    dump_bytes_as_hex(stream, buffer.begin(), writer.valid());
    std::cout << "\n";

    return document.ok();
  }

  bool do_array_concrete(std::ostream& stream) __attribute__((noinline));
  bool do_array_concrete(std::ostream& stream) {
    // { "array" : [ "hello", "world" ] }
    std::array<char, 128> buffer;
    auto writer = make_array_writer(buffer);
    concrete_encoder<decltype(writer)> document(writer);
    make_array_encoder(document.start_subarray("array"))
      .encode_utf8_string("hello")
      .encode_utf8_string("world")
      .finish();
    document.finish();

    dump_bytes_as_hex(stream, buffer.begin(), writer.valid());
    std::cout << "\n";

    return document.ok();
  }

} // namespace

int main(int argc, char* argv[]) {
  do_example(std::cout);
  do_nested_example(std::cout);
  do_nested_concrete(std::cout);
  do_array(std::cout);
  do_array_manually(std::cout);
  do_array_concrete(std::cout);
  return EXIT_SUCCESS;
}

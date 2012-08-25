#include <bassoon/encoder.hpp>

#include <array>
#include <gtest/gtest.h>

namespace {

  class Foo {
  public:
    bool do_thing() { return true; }
    bool do_other_thing(int) { return true; }
  };

  int get_last_error() { return 1; }

  template<typename T>
  struct xcp_wrapper {
    xcp_wrapper(T& t) : t_(t) {}

    template<typename U>
    invoker<U> invoker(U u) {
      return invoker(t_, 
    }

    template<typename U, typename ...ArgTypes>
    xcp_wrapper& invoke(U u, ArgTypes&& ...args) {
      auto result = (t_.*u)(std::forward<ArgTypes>(args)...);
      if( not result )
        throw get_last_error();
      return *this;
    }

    T& t_;
  };

  template<typename T>
  xcp_wrapper<T> make_xcp_wrapper(T& t) {
    return xcp_wrapper<T>(t);
  }



  using namespace bassoon::bson;

  TEST(HelloWorldTest, Encode) {

  Foo f;
  auto ff = make_xcp_wrapper(f);
  ff
    .invoke(&decltype(f)::do_thing)
    .invoke(&Foo::do_other_thing, 2);
#if 0
    const char buffer[] = {
      0x16, 0x00, 0x00, 0x00,
      0x02,
      'h', 'e', 'l', 'l', 'o', 0x00,
      0x06, 0x00, 0x00, 0x00,  'w', 'o', 'r', 'l', 'd', 0x00,
      0x00,
    };

    auto writer = make_validating_writer(buffer);
    EXPECT_TRUE(writer.ok());

    auto document = start_document(writer);
    EXPECT_TRUE(writer.ok());

    document.encode_utf8_string("hello", "world");
    EXPECT_TRUE(writer.ok());
#endif
  }

}

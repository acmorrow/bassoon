#ifndef included_790cd424_d203_44d1_b557_dd1df7b76613
#define included_790cd424_d203_44d1_b557_dd1df7b76613

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <string>
#include <type_traits>

#include <bassoon/binary_data.hpp>

namespace bassoon {
  namespace bson {

    namespace string_data_details {
      struct literal_tag {};
      struct null_included_tag {};
    }  // namespace string_data_details

    template<typename P, typename S>
    struct generic_string_data : public generic_binary_data<P, S> {

      using base_type = generic_binary_data<P, S>;
      using value_type = typename base_type::value_type;
      using pointer_type = typename base_type::pointer_type;
      using size_type = typename base_type::size_type;

      static_assert(std::is_same<const char, const value_type>::value,
                    "generic_string_data is for 'char' and 'const char' only");

      // NOTE(acm): It is critical that 'size' incorporates the NULL
      // byte. So for the string "cat", the 'size' value should be 4,
      // not 3 like strlen would say.
      generic_string_data(pointer_type data_, size_type size_, string_data_details::null_included_tag)
        : base_type(data_, size_) {
        assert(this->data[this->size - 1] == '\0');
      }

      // Use this if you already know what std::strlen(str) is. It
      // will adjust the count for you.
      generic_string_data(pointer_type str, std::size_t strlen_str)
        : generic_string_data(str, check_size(strlen_str + 1), string_data_details::null_included_tag()) {}

      // Use this if you don't know how long the string is yet. It will
      // call std::strlen for you.
      generic_string_data(pointer_type str)
        : generic_string_data(str, std::strlen(str)) {}

      // NOTE(acm): Assumes that std::string::c_str() is efficient. I
      // believe it is for all modern C++ standard libraries.
      generic_string_data(std::string const& str)
        : generic_string_data(str.c_str(), str.length()) {}

      // NOTE(acm): In reality, this is unlikely to be selected.
      template<std::size_t N>
      generic_string_data(value_type (&str)[N])
      : generic_string_data(str, string_data_details::literal_tag()) {}

      // NOTE(acm): You can use a tag type to select it directly if
      // you have a literal but the compiler is not deciding to
      // optimize away the strlen call.
      template<std::size_t N>
      generic_string_data(value_type (&str)[N], string_data_details::literal_tag)
        : generic_string_data(&str[0], N) {}

      template<typename other_size_type>
      static size_type check_size(other_size_type size) {
        if (not std::is_same<size_type, other_size_type>::value) {
          if (std::numeric_limits<size_type>::max() < size) {
            // TODO(acm):
            // error_context().on_string_overflow();
            std::abort();
          }
        }
        return size;
      }
    };

    // BSON 'string' lengths are constrained to int32_t size.
    using string_data = generic_string_data<char, length_t>;
    using string_cdata = generic_string_data<char const, length_t>;

    // BSON 'cstring' lengths are not constrained by the spec.
    // TODO(acm): Add a configurable max length.
    // TODO(acm): Make cstring stuff check for embedded nulls in debug mode.
    using cstring_data = generic_string_data<char, std::size_t>;
    using cstring_cdata = generic_string_data<char const, std::size_t>;

  }  // namespace bson
}  // namespace bassoon

#endif // included_790cd424_d203_44d1_b557_dd1df7b76613

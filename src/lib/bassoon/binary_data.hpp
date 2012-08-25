#ifndef included_aac9c4fd_b3ec_4c13_843a_a9e953659e64
#define included_aac9c4fd_b3ec_4c13_843a_a9e953659e64
#pragma once

#include <type_traits>
#include <bassoon/bson.hpp>

namespace bassoon {
  namespace bson {

    template<typename T, typename S>
    struct generic_binary_data {
      using value_type = T;
      using pointer_type = typename std::add_pointer<value_type>::type;
      using size_type = S;

      // TODO(acm): Static asserts on value_type/pointer_type
      static_assert(std::is_integral<size_type>::value, "Size type must be integral");

      // TODO(acm): Figure out more about when noexcept is deduced.
      constexpr generic_binary_data() noexcept = default;

      constexpr generic_binary_data(pointer_type data_, size_type size_) noexcept
        : data(data_)
        , size(size_ >= 0 ? size_ : 0) {}  // TODO(acm): Fix warning

      pointer_type const data;
      size_type const size;
    };

    template<typename T, std::size_t fixed_size>
    struct fixed_length_binary_data : public generic_binary_data<T, std::size_t> {

      using base_type = generic_binary_data<T, std::size_t>;
      using value_type = typename base_type::value_type;
      using pointer_type = typename base_type::pointer_type;

      constexpr fixed_length_binary_data(pointer_type data) noexcept
        : base_type(data, fixed_size) {}

      constexpr fixed_length_binary_data(value_type (&data)[fixed_size]) noexcept
        : base_type(data, fixed_size) {}
    };

    // BSON documents and binary objects are constrained to int32_t size.
    using binary_data = generic_binary_data<void, std::int32_t>;
    using binary_cdata = generic_binary_data<void const, std::int32_t>;

    // BSON object ID's are a fixed length array of byte_t's.
    using object_id_data = fixed_length_binary_data<byte_t, k_object_id_length>;
    using object_id_cdata = fixed_length_binary_data<byte_t const, k_object_id_length>;

  }  // namespace bson
}  // namespace bassoon

#endif // included_aac9c4fd_b3ec_4c13_843a_a9e953659e64

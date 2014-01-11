#ifndef included_6bf34bab_ade9_4622_9af8_c7e5c3345325
#define included_6bf34bab_ade9_4622_9af8_c7e5c3345325

namespace bassoon {
  namespace bson {

    template<typename T, typename S>
    struct generic_document_data : generic_binary_data<T, S> {

      using base_type = generic_binary_data<T, S>;
      using value_type = typename base_type::value_type;
      using pointer_type = typename base_type::pointer_type;
      using size_type = typename base_type::size_type;

      generic_document_data(pointer_type data)
        : generic_binary_data(data, get_size(data)) {}

    };

    using document_data = generic_document_data<void, int32>;
    using document_cdata = generic_document_data<void const, int32>;

  }  // namespace bson
}  // namespace bassoon

#endif // included_6bf34bab_ade9_4622_9af8_c7e5c3345325

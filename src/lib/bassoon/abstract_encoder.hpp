#ifndef included_175af189_2f98_496e_b425_890c80698069
#define included_175af189_2f98_496e_b425_890c80698069

#include <bassoon/encoder_interface.hpp>

namespace bassoon {
  namespace bson {

    class LIBBASSOON_EXPORT abstract_encoder : public encoder_interface {
    public:
        virtual ~abstract_encoder() = default;

        ///
        /// Starts a new subdocument with the name 'name'.
        ///
        virtual abstract_encoder& start_subdocument(cstring_cdata name) = 0;

        ///
        /// Starts a new subarray with the name 'name'.
        ///
        virtual abstract_encoder& start_subarray(cstring_cdata name) = 0;

        ///
        /// Finishes the current document. You must call this once for every call
        /// to 'start_subdocument' or 'start_subarray', plus one extra time to
        /// finish the top-level document.
        ///
        virtual abstract_encoder& finish() = 0;

    protected:
        abstract_encoder() = default;
    };

  }  // namespace bson
}  // namespace bassoon

#endif // included_175af189_2f98_496e_b425_890c80698069

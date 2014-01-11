#ifndef included_596bb6f0_d143_43d5_bf8e_cc2635f210f0
#define included_596bb6f0_d143_43d5_bf8e_cc2635f210f0

// It is sad that we are still doing this in 2014 :(
#if defined(__linux__)
#  include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define le16toh(x) letoh16(x)
#  define le32toh(x) letoh32(x)
#  define le64toh(x) letoh64(x)
#elif defined(__APPLE__)
#  include <libkern/OSByteOrder.h>
#  define le16toh(x) OSSwapLittleToHostInt16(x)
#  define le32toh(x) OSSwapLittleToHostInt32(x)
#  define le64toh(x) OSSwapLittleToHostInt64(x)
#  define htole16(x) OSSwapHostToLittleInt16(x)
#  define htole32(x) OSSwapHostToLittleInt32(x)
#  define htole64(x) OSSwapHostToLittleInt64(x)
#endif

#include <cinttypes>

namespace bassoon {
namespace endian {

    template<typename T>
    struct converter;

    template<>
    struct converter<std::uint8_t> {

        using T = std::uint8_t;

        inline static T native_to_little(T t) {
            return t;
        }

        inline static T little_to_native(T t) {
            return t;
        }
    };

    template<>
    struct converter<std::uint16_t> {

        using T = std::uint16_t;

        inline static T native_to_little(T t) {
            return htole16(t);
        }

        inline static T little_to_native(T t) {
            return le16toh(t);
        }
    };

    template<>
    struct converter<std::uint32_t> {

        using T = std::uint32_t;

        inline static T native_to_little(T t) {
            return htole32(t);
        }

        inline static T little_to_native(T t) {
            return le32toh(t);
        }
    };

    template<>
    struct converter<std::uint64_t> {

        using T = std::uint64_t;

        inline static T native_to_little(T t) {
            return htole64(t);
        }

        inline static T little_to_native(T t) {
            return le64toh(t);
        }
    };

    template<typename T>
    T native_to_little(T t) {
        typedef typename std::make_unsigned<T>::type UT;
        return converter<UT>::native_to_little(*reinterpret_cast<UT*>(&t));
    }

    template<typename T>
    T little_to_native(T t) {
        typedef typename std::make_unsigned<T>::type UT;
        return converter<UT>::native_to_little(*reinterpret_cast<UT*>(&t));
    }

} // namespace endian
} // namespace bassoon

#endif // included_596bb6f0_d143_43d5_bf8e_cc2635f210f0

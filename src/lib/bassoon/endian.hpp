#ifndef included_596bb6f0_d143_43d5_bf8e_cc2635f210f0
#define included_596bb6f0_d143_43d5_bf8e_cc2635f210f0

// It is sad that we are still doing this in 2014 :(
// At some point we should just write our own.
#if defined(__linux__)
#  include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define be16toh(x) betoh16(x)
#  define be32toh(x) betoh32(x)
#  define be64toh(x) betoh64(x)
#  define le16toh(x) letoh16(x)
#  define le32toh(x) letoh32(x)
#  define le64toh(x) letoh64(x)
#elif defined(__APPLE__)
#  include <libkern/OSByteOrder.h>
#  define be16toh(x) OSSwapBigToHostInt16(x)
#  define be32toh(x) OSSwapBigToHostInt32(x)
#  define be64toh(x) OSSwapBigToHostInt64(x)
#  define htobe16(x) OSSwapHostToBigInt16(x)
#  define htobe32(x) OSSwapHostToBigInt32(x)
#  define htobe64(x) OSSwapHostToBigInt64(x)
#  define le16toh(x) OSSwapLittleToHostInt16(x)
#  define le32toh(x) OSSwapLittleToHostInt32(x)
#  define le64toh(x) OSSwapLittleToHostInt64(x)
#  define htole16(x) OSSwapHostToLittleInt16(x)
#  define htole32(x) OSSwapHostToLittleInt32(x)
#  define htole64(x) OSSwapHostToLittleInt64(x)
#elif defined(_WIN32)
// TODO(acm): This should be fixed some day.
#error
#endif

#include <cinttypes>

namespace bassoon {
namespace endian {

    template<typename T>
    struct converter;

    template<>
    struct converter<std::uint8_t> {

        using T = std::uint8_t;

        inline static T native_to_big(T t) {
            return t;
        }

        inline static T big_to_native(T t) {
            return t;
        }

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

        inline static T native_to_big(T t) {
            return htobe16(t);
        }

        inline static T big_to_native(T t) {
            return be16toh(t);
        }

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

        inline static T native_to_big(T t) {
            return htobe32(t);
        }

        inline static T big_to_native(T t) {
            return be32toh(t);
        }

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

        inline static T native_to_big(T t) {
            return htobe64(t);
        }

        inline static T big_to_native(T t) {
            return be64toh(t);
        }

        inline static T native_to_little(T t) {
            return htole64(t);
        }

        inline static T little_to_native(T t) {
            return le64toh(t);
        }
    };

    // Please see http://stackoverflow.com/questions/1751346/interpret-signed-as-unsigned
    // for why we use static_cast here and in the other _to_ functions.

    template<typename T>
    T native_to_big(T t) {
        typedef typename std::make_unsigned<T>::type UT;
        return converter<UT>::native_to_big(static_cast<UT>(t));
    }

    template<typename T>
    T big_to_native(T t) {
        typedef typename std::make_unsigned<T>::type UT;
        return converter<UT>::big_to_native(static_cast<UT>(t));
    }

    template<typename T>
    T native_to_little(T t) {
        typedef typename std::make_unsigned<T>::type UT;
        return converter<UT>::native_to_little(static_cast<UT>(t));
    }

    template<typename T>
    T little_to_native(T t) {
        typedef typename std::make_unsigned<T>::type UT;
        return converter<UT>::little_to_native(static_cast<UT>(t));
    }

    // TODO: These two don't really belong here since we aren't doing networking, but it seems
    // best to include them.

    template<typename T>
    T host_to_net(T t) {
        return native_to_big(t);
    }

    template<typename T>
    T net_to_host(T t) {
        return big_to_native(t);
    }

} // namespace endian
} // namespace bassoon

#endif // included_596bb6f0_d143_43d5_bf8e_cc2635f210f0

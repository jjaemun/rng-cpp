#include <gtest/gtest.h>
#include "rng/dist/canon.hpp"


#define half_open_lowerbound(utype, ftype)                             \
    static_assert(                                                     \
        rng::dist::canon_half_open_from_unsigned_bits<ftype>(          \
            utype{0}                                                   \
        ) == ftype{0}                                                  \
    )

#define half_open_upperbound(utype, ftype)                             \
    static_assert(                                                     \
        rng::dist::canon_half_open_from_unsigned_bits<ftype>(          \
            rng::MAX<utype>                                            \
        ) == (ftype{1} - rng::EPSILON<ftype> / ftype{2})               \
    )

#define half_open_midpoint(utype, ftype)                               \
    static_assert(                                                     \
        rng::dist::canon_half_open_from_unsigned_bits<ftype>(          \
            utype{1} << (rng::DIGITS<utype> - 1)                       \
        ) == ftype{0.5}                                                \
    )

#define half_open_ignores_excess(utype, ftype)                         \
    static_assert(                                                     \
        rng::dist::canon_half_open_from_unsigned_bits<ftype>(          \
            (utype{1}                                                  \
                << (rng::DIGITS<utype> - rng::DIGITS<ftype>)) -        \
                utype{1}                                               \
        ) == ftype{0}                                                  \
    )

#define open_lowerbound(utype, ftype)                                  \
    static_assert(                                                     \
        rng::dist::canon_open_from_unsigned_bits<ftype>(               \
            utype{0}                                                   \
        ) == (rng::EPSILON<ftype> / ftype{2})                          \
    )

#define open_upperbound(utype, ftype)                                  \
    static_assert(                                                     \
        rng::dist::canon_open_from_unsigned_bits<ftype>(               \
            rng::MAX<utype>                                            \
        ) == (ftype{1} - rng::EPSILON<ftype> / ftype{2})               \
    )

#define open_excludes_midpoint(utype, ftype)                           \
    static_assert(                                                     \
        rng::dist::canon_open_from_unsigned_bits<ftype>(               \
            utype{1} << (rng::DIGITS<utype> - 1)                       \
        ) == (ftype{0.5} + rng::EPSILON<ftype> / ftype{2})             \
    )

#define open_is_symmetric(utype, ftype)                                \
    static_assert(                                                     \
        rng::dist::canon_open_from_unsigned_bits<ftype>(               \
            rng::MAX<utype> / utype{3}                                 \
        ) +                                                            \
        rng::dist::canon_open_from_unsigned_bits<ftype>(               \
            ~(rng::MAX<utype> / utype{3})                              \
        ) == ftype{1}                                                  \
    )


namespace {
    using namespace rng;

    TEST(canon_tests, SufficientBitSources) {
        static_assert(
            dist::SufficientBitSource<u32, f32>
        );

        static_assert(
            dist::SufficientBitSource<u64, f32>
        );

        static_assert(
            dist::SufficientBitSource<u64, f64>
        );

        static_assert(
            !dist::SufficientBitSource<u32, f64>
        );
    }


    TEST(canon_tests, HalfOpenIncludesLowerBound) {
        half_open_lowerbound(u32, f32);
        half_open_lowerbound(u64, f64);
    }


    TEST(canon_tests, HalfOpenExcludesUpperBound) {
        half_open_upperbound(u32, f32);
        half_open_upperbound(u64, f64);
    }


    TEST(canon_tests, HalfOpenContainsMidpoint) {
        half_open_midpoint(u32, f32);
        half_open_midpoint(u64, f64);
    }


    TEST(canon_tests, HalfOpenIgnoresExcessBits) {
        half_open_ignores_excess(u32, f32);
        half_open_ignores_excess(u64, f64);
    }


    TEST(canon_tests, OpenExcludesLowerBound) {
        open_lowerbound(u32, f32);
        open_lowerbound(u64, f64);
    }


    TEST(canon_tests, OpenExcludesUpperBound) {
        open_upperbound(u32, f32);
        open_upperbound(u64, f64);
    }


    TEST(canon_tests, OpenExcludesMidpoint) {
        open_excludes_midpoint(u32, f32);
        open_excludes_midpoint(u64, f64);
    }


    TEST(canon_tests, OpenIsSymmetric) {
        open_is_symmetric(u32, f32);
        open_is_symmetric(u64, f64);
    }
}


#undef half_open_lowerbound
#undef half_open_upperbound
#undef half_open_midpoint
#undef half_open_ignores_excess
#undef open_lowerbound
#undef open_upperbound
#undef open_excludes_midpoint
#undef open_is_symmetric

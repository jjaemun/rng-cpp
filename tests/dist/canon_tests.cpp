#include <gtest/gtest.h>

#include "rng/dist/canon.hpp"


#define lowerbound(utype, ftype)                                       \
    static_assert(                                                     \
        rng::dist::canon_half_open_from_unsigned_bits<ftype>(          \
            utype{0}                                                   \
        ) == ftype{0}                                                  \
    )

#define upperbound(utype, ftype)                                       \
    static_assert(                                                     \
        rng::dist::canon_half_open_from_unsigned_bits<ftype>(          \
            rng::MAX<utype>                                            \
        ) == (ftype{1} - rng::EPSILON<ftype> / ftype{2})               \
    )


namespace {
    using namespace rng;

    TEST(canon_tests, IncludesLowerBound) {
        lowerbound(u32, f32);
        lowerbound(u64, f64);
    }

    TEST(canon_tests, ExcludesUpperBound) {
        upperbound(u32, f32);
        upperbound(u64, f64);
    }
}


#undef lowerbound
#undef upperbound

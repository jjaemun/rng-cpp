#include <array>
#include <gtest/gtest.h>


#include "rng/rngs/xoshiro.hpp"


#define TRY_DEREF(opt) opt.has_value()


namespace {
    using namespace rng;

    TEST(xoshiro_tests, NullStateInvalid) {
        constexpr std::array<u64, 4> nullish{
            0, 0,
            0, 0
        };

        EXPECT_FALSE(TRY_DEREF(rngs::Xoshiro256PlusPlus::seed_from_state(nullish)));
    }

    TEST(xoshiro_tests, NonNullStateValid) {
        constexpr std::array<u64, 4> notnull{
            0, 0,
            0, 1
        };

        EXPECT_TRUE(TRY_DEREF(rngs::Xoshiro256PlusPlus::seed_from_state(notnull)));
    }

    TEST(xoshiro_tests, MatchesReferenceSequence) {
        constexpr std::array<u64, 4> words{
            1, 2,
            3, 4
        };

        auto gen = rngs::Xoshiro256PlusPlus::seed_from_state(words);
        ASSERT_TRUE(TRY_DEREF(gen));

        constexpr std::array<u64, 10> truths{
            0x0000000002800001ULL,
            0x0000000003800067ULL,
            0x000cc00003800067ULL,
            0x000cc201994400b2ULL,
            0x8012a2019ac433cdULL,
            0x8a69978acdee33baULL,
            0xc271134733154abdULL,
            0xac2ba09179169e97ULL,
            0xdbf3190a8f073fd8ULL,
            0x9105f14ab2229220ULL,
        };

        for (auto truth : truths)
            EXPECT_EQ(gen->next_u64(), truth);
    }
    
    TEST(xoshiro_tests, UsesUpperBits) {
        auto gen32 = rngs::Xoshiro256PlusPlus::seed_from_u64(42);
        auto gen64 = rngs::Xoshiro256PlusPlus::seed_from_u64(42);

        const u32 expected = 
            static_cast<u32>(gen64.next_u64() >> 32);

        EXPECT_EQ(gen32.next_u32(), expected);
        EXPECT_EQ(gen32.next_u64(), gen64.next_u64());
        //        ^^^^^^^^^^^^^^    ^^^^^^^^^^^^^^
        //        |
        //        - Confirm we have advanced state once.
    }
}


#undef TRY_DEREF

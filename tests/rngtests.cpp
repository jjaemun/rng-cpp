#include <random>
#include <gtest/gtest.h>

#include "rng/rng.hpp"
#include "rng/rngs/xoshiro.hpp"


namespace {
    using namespace rng;

    template <typename... Generators>
        requires ((Rng<Generators>) && ...)
    constexpr bool rngtypes = true;

    TEST(rngtests, RngConcept) {
        static_assert(
            rngtypes<
                rngs::Xoshiro256PlusPlus
            >
        );
    }

    template <typename... Generators>
        requires ((std::uniform_random_bit_generator<Generators>) && ...)
    constexpr bool urbgtypes = true;

    TEST(rngtests, UniformRbgConcept) {
        static_assert(
            urbgtypes<
                rngs::Xoshiro256PlusPlus
            >
        );
    }
}

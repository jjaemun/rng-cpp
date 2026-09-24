#pragma once


#include <concepts>
#include <span>

#include "rng/types.hpp"


namespace rng {
    // Models base concept for random bit generators.  
    //
    // Generators satisfying `Rng` should be deterministic sources 
    // of uniform random bits.
    template <typename R>
    concept Rng = requires (R& gen, std::span<std::byte> dst) {
        // Generates the next random `u32`.
        { gen.next_u32() } -> std::same_as<u32>;

        // Generates the next random `u64`.
        { gen.next_u64() } -> std::same_as<u64>;

        // Fills caller-owned byte buffer. 
        //
        // It provides an allocation-free api for random number generation.
        { gen.fill_bytes(dst) } -> std::same_as<void>;
    };
} // namespace rng

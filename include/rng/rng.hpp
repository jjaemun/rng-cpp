#pragma once


#include <concepts>
#include <span>

#include "rng/types.hpp"


namespace rng {
    template <typename R>
    concept Rng = requires (R& gen, std::span<std::byte> dst) {
        { gen.next_u32() } -> std::same_as<u32>;
        { gen.next_u64() } -> std::same_as<u64>;
        { gen.fill_bytes(dst) } -> std::same_as<void>;
    };

} // namespace rng

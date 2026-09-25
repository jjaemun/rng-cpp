#pragma once


#include <concepts>

#include "rng/num/concepts.hpp"
#include "rng/num/traits.hpp"
#include "rng/types.hpp"


namespace rng::sealed {
    template <typename T>
    concept Sealed = (std::same_as<T, f32> || std::same_as<T, f32>)
                        && (RADIX<T> == 2u);
} // namespace rng::sealed

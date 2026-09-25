
#pragma once


#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <span>


#include "rng/num/concepts.hpp"
#include "rng/num/traits.hpp"
#include "rng/rng.hpp"
#include "rng/sealed.hpp"


namespace rng::dist {
    // `SufficientBitSource` guarantees that the random unsigned bit source
    // can supply enough bits to construct a canonical uniformly distributed target.
    template <typename T, typename U>
        requires (Float<T> && !Signed<U>)
    concept SufficientBitSource = (DIGITS<T> <= DIGITS<U>);


    template <typename T, typename U>
        requires (SufficientBitSource<T, U>)
    [[nodiscard]]
    constexpr T canon_half_open_from_unsigned_bits(U bits) noexcept {
        const auto fdgs = DIGITS<T>;
        const auto udgs = DIGITS<U>;

        const auto factor = EPSILON<T> / T{2};

        return static_cast<T>(bits >> (udgs - fdgs)) * factor;
    }

    template <typename T, typename U>
        requires (SufficientBitSource<U, T>)
    [[nodiscard]]
    constexpr T canon_open_from_unsigned_bits(U bits) noexcept {
        const auto fdgs = DIGITS<T>;
        const auto udgs = DIGITS<U>;

        const auto factor = EPSILON<T> / T{2};

        const auto fixed = 
            (bits >> (udgs - fdgs) | U{1});

        return static_cast<T>(fixed) * factor;
    }
}

#pragma once


#include <concepts>
#include "rng/rng.hpp"


namespace rng::dist {
    // Return type produced by sampling a distribution `D` endowed with some generator `R`.
    template <typename D, typename R>
        requires (Rng<R>)
    using Sample = decltype(std::declval<D&>().sample(std::declval<&R>()));

    // Models a statistical distribution.
    template <typename D, typename R>
        requires (Rng<R>)
    concept Distribution = requires (D& distr, R& gen, 
                                     std::span<Sample<D, R> dst)  {
        // Samples a single element from the given distribution.
        { distr.sample(gen) } -> std::same_as<Sample<D, R>>;
        // Fills a contiguous buffer, `dist`, with random samples from the given distribution.
        { distr.fill(gen, dst) } -> std::same_as<void>;
    };
} // namespace rng::dist

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
    template <typename T, typename U>
        requires (Float<T> && !Signed<U>)
    concept SufficientBitSource = (DIGITS<T> <= DIGITS<U>);


    // `SufficientBitSource` guarantees that the random unsigned bit source
        // can supply enough bits to construct a canonical uniformly distributed target.
    template <typename T, typename U>
        requires (SufficientBitSource<T, U>)
    [[nodiscard]]
    constexpr T canon_from_unsigned_bits(U bits) noexcept {
        const auto fdgs = DIGITS<T>;
        const auto udgs = DIGITS<U>;

        const auto factor = EPSILON<T> / T{2};

        return static_cast<T>(bits >> (udgs - fdgs)) * factor;
    }

    template <typename S>
        requires (sealed::Sealed<S>)
    class Uniform final {

        S a;
        S b;

    private:
        explicit constexpr Uniform(S a_, S b_) noexcept
            : a(a_), b(b_) {}

    public:
        [[nodiscard]]
        static constexpr Uniform from_bounds_unchecked(S lower, S upper) noexcept {
            return Uniform{lower, upper};
        }

        [[nodiscard]]
        static auto from_bounds(S lower, S upper) noexcept 
            -> std::optional<Uniform> 
        {
            if (!std::isfinite(lower) 
                    || !std::isfinite(upper))
                return std::nullopt;
            
            if (lower >= upper)
                return std::nullopt;

            return Uniform{lower, upper};
        }

        template <typename R>
            requires (Rng<R>)
        [[nodiscard]]
        S sample(R& gen) const noexcept {
            S seal;
            if constexpr (std::same_as<S, f32>) {
                seal = canon_from_unsigned_bits<S>(gen.next_u32());
            } else {
                seal = canon_from_unsigned_bits<S>(gen.next_u64());
            }
            
            const S sampled = std::lerp(a, b, seal);

            if (sampled < b) {
                return sampled;
            }

            return std::nextafter(b, a);
        }

        template <typename R>
            requires (Rng<R>)
        void fill(R& gen, std::span<S> dst) const noexcept {
            for (auto& item : dst) {
                item = sample(gen);
            }
        }
    };
} // namespace rng::dist

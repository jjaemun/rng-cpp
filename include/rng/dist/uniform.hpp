#pragma once


#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <span>


#include "rng/concepts.hpp"
#include "rng/rng.hpp"
#include "rng/types.hpp"


namespace rng::dist {
    // Converts uniformly distributed bits into a floating-point value in [0, 1).
    template <FpType F, UnsignedIntType U>
    [[nodiscard]]
    constexpr F canonical(U bits) noexcept {
        constexpr auto floating_digits =
            std::numeric_limits<F>::digits;

        constexpr auto integer_digits =
            std::numeric_limits<U>::digits;

        static_assert(
            floating_digits <= integer_digits,
            "The integer type does not provide enough random bits."
        );

        constexpr auto excess =
            integer_digits - floating_digits;

        constexpr F scale =
            EPSILON<F> / F{2};

        return static_cast<F>(bits >> excess) * scale;
    }


    template <typename T>
    concept UniformType =
        std::same_as<T, f32> ||
        std::same_as<T, f64>;


    template <UniformType T>
    class Uniform final {

        T lower;
        T upper;

    private:
        explicit Uniform(T lower_, T upper_) noexcept
            : lower(lower_),
              upper(upper_) {}

    public:
        [[nodiscard]]
        static auto from_bounds(T lower, T upper) noexcept
            -> std::optional<Uniform>
        {
            if (!std::isfinite(lower) || !std::isfinite(upper)) {
                return std::nullopt;
            }

            if (lower >= upper) {
                return std::nullopt;
            }

            return Uniform{lower, upper};
        }

        template <typename R>
            requires (Rng<R>)
        [[nodiscard]]
        T sample(R& gen) const noexcept {
            T unit;

            if constexpr (std::same_as<T, f32>) {
                unit = canonical<T>(gen.next_u32());
            } else {
                unit = canonical<T>(gen.next_u64());
            }

            const T sampled =
                std::lerp(lower, upper, unit);

            if (sampled < upper) {
                return sampled;
            }

            return std::nextafter(upper, lower);
        }

        template <typename R>
            requires (Rng<R>)
        void fill(R& gen, std::span<T> dst) const noexcept {
            for (auto& value : dst) {
                value = sample(gen);
            }
        }
    };
} // namespace rng::dist

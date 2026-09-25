#pragma once


#include <cmath>
#include <concepts>
#include <limits>
#include <optional>
#include <span>


#include "rng/num/concepts.hpp"
#include "rng/num/traits.hpp"
#include "rng/rng.hpp"
#include "rng/types.hpp"


namespace rng::dist {
    // Converts uniformly distributed bits into a floating-point value in [0, 1).
    template <typename T, typename U>
        requires (num::FpType<T> 
                        && num::UnsignedIntType<U>)
    [[nodiscard]]
    constexpr T canonical(U bits) noexcept {
        using namespace num;

        constexpr auto fpdig = DIGITS<T>;
        constexpr auto indig = DIGITS<U>;

        static_assert(fpdig <= indig);

        // auxiliaries.
        constexpr auto excess = indig - fpdig;
        constexpr auto scale = EPSILON<T> / T{2};

        return static_cast<T>(bits >> excess) * scale;
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

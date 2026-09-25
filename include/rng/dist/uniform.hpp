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
    constexpr T canon_from_unsigned_bits(U bits) noexcept {
        using namespace num;

        static_assert(DIGITS<T> <= DIGITS<U>);

        constexpr auto excess = DIGITS<U> - DIGITS<T>;
        constexpr auto factor = EPSILON<T> / T{2};

        return static_cast<T>(bits >> excess) * factor;
    }

    
    namespace sealed {
        template <typename T>
        concept Sealed = (std::same_as<T, f32> 
                                || std::same_as<T, f64>);
    } // namespace sealed

    template <typename S>
        requires (sealed::Sealed<S>)
    class Uniform final {

        S a;
        S b;

    private:
        explicit Uniform(S a_, S b_) noexcept
            : a(a_), b(b_) {}

    public:
        [[nodiscard]]
        static std::optional<Uniform> from_bounds(S lower, S upper) noexcept {
            if (!std::isfinite(lower) || !std::isfinite(upper))
                return std::nullopt;
            
            if (lower >= upper)
                return std::nullopt;

            return Uniform{lower, upper};
        }

        template <typename R>
            requires (Rng<R>)
        [[nodiscard]]
        S sample(R& gen) const noexcept {
            S s;

            if constexpr (std::same_as<S, f32>)
                s = canon_from_unsigned_bits<S>(gen.next_u32());
            else 
                s = canon_from_unsigned_bits<S>(gen.next_u64());
            
            const S sample = std::lerp(a, b, unit);

            if (sample < b)
                return sample;
            
            return std::nextafter(a, b);
        }

        template <typename R>
            requires (Rng<R>)
        void fill(R& gen, std::span<S> dst) const noexcept {
            for (auto& e : dst) {
                e = sample(gen);
            }
        }
    };
} // namespace rng::dist

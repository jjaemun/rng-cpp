#pragma once


#include <limits>
#include "rng/num/concepts.hpp"


namespace rng::num {
    template <typename T>
        requires (IntType<T> || FpType<T>)
    inline constexpr auto MAX{std::numeric_limits<T>::max()};

    template <typename T>
        requires (FpType<T>)
    inline constexpr auto MIN_POSITIVE{std::numeric_limits<T>::min()}; 

    template <typename T>
        requires (IntType<T> || FpType<T>)
    inline constexpr auto MIN{std::numeric_limits<T>::lowest()};

    template <typename T>
        requires (IntType<T> || FpType<T>)
    inline constexpr auto BYTES{sizeof(T)};

    template <typename T>
        requires (IntType<T> || FpType<T>)
    inline constexpr auto BITS{sizeof(T) * 8u};

    template <typename T>
        requires (FpType<T>)
    inline constexpr auto INF{std::numeric_limits<T>::infinity()};

    template <typename T>
        requires (FpType<T>)
    inline constexpr auto NEG_INF{-std::numeric_limits<T>::infinity()};

    template <typename T>
        requires (FpType<T>)
    inline constexpr auto RADIX{std::numeric_limits<T>::radix};

    template <typename T>
        requires (FpType<T> || IntType<T>)
    inline constexpr auto DIGITS{std::numeric_limits<T>::digits};

    template <typename T>
        requires (FpType<T>)
    inline constexpr auto QUIET_NAN{std::numeric_limits<T>::quiet_NaN()};
        
    template <typename T>
        requires (FpType<T>)
    inline constexpr auto EPSILON{std::numeric_limits<T>::epsilon()};
} //namespace rng::num

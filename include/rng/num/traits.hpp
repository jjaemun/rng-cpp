#pragma once


#include <limits>
#include "rng/num/concepts.hpp"


namespace rng::traits {
    template <typename T>
        requires (IntType<T> || FpType<T>)
    static constexpr auto MAX{std::numeric_limits<T>::max()};

    template <typename T>
        requires (FpType<T>)
    static constexpr auto MIN_POSITIVE{std::numeric_limits<T>::min()}; 

    template <typename T>
        requires (IntType<T> || FpType<T>)
    static constexpr auto MIN{std::numeric_limits<T>::lowest()};

    template <typename T>
        requires (IntType<T> || FpType<T>)
    static constexpr auto BYTES{sizeof(T)};

    template <typename T>
        requires (IntType<T> || FpType<T>)
    static constexpr auto BITS{sizeof(T) * 8uz};

    template <typename T>
        requires (FpType<T>)
    static constexpr auto INF{std::numeric_limits<T>::infinity()};

    template <typename T>
        requires (FpType<T>)
    static constexpr auto NEG_INF{-std::numeric_limits<T>::infinity()};

    template <typename T>
        requires (FpType<T>)
    static constexpr auto RADIX{std::numeric_limits<T>::radix};

    template <typename T>
        requires (FpType<T> || IntType<T>)
    static constexpr auto DIGITS{std::numeric_limits<T>::digits10};

    template <typename T>
        requires (FpType<T>)
    static constexpr auto MAX_DIGITS{std::numeric_limits<T>::max_digits10};

    template <typename T>
        requires (FpType<T>)
    static constexpr auto QUIET_NAN{std::numeric_limits<T>::quiet_NaN()};
        
    template <typename T>
        requires (FpType<T>)
    static constexpr auto EPSILON{std::numeric_limits<T>::epsilon()};
} //namespace rng

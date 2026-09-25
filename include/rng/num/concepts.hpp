#pragma once


#include <concepts>
#include <type_traits>


namespace rng::num {
    // Satisfied by integral scalar types.
    // 
    // `IntType<T>` holds iff `T` models `std::integral<T>`.
    //
    // # Examples
    //
    //```c++
    //
    // // compiles.
    // static_assert(IntType<i32>);
    // // fails.
    // static_assert(IntType<f64>);
    // ```
    template <typename T>
    concept IntType = std::integral<T>;

    // Satisfied by signed integral types.
    // 
    // `SignedIntType<T>` holds iff `T` models both `std::integral<T>`
    // and `std::signed_integral<T>`.
    //
    // # Examples
    //
    //```c++
    //
    // // compiles.
    // static_assert(SignedIntType<i32>);
    // // fails.
    // static_assert(SignedIntType<u32>);
    // ```
    template <typename T>
    concept SignedIntType = IntType<T> && std::signed_integral<T>;

    // Satisfied by unsigned integral types.
    // 
    // `UnsignedIntType<T>` holds iff `T` models `std::integral<T>`
    // but *not* `std::signed_integral<T>`.
    //
    // # Examples
    //
    //```c++
    //
    // // compiles.
    // static_assert(UnsignedIntType<u32>);
    // // fails.
    // static_assert(UnsignedIntType<i32>);
    // ```
    template <typename T>
    concept UnsignedIntType = IntType<T> && !std::signed_integral<T>;

    // Satisfied by floating point scalar types.
    // 
    // `FpType<T>` holds iff `T` models `std::floating_point<T>`
    //
    // # Examples
    //
    //```c++
    //
    // // compiles.
    // static_assert(FpType<f32>);
    // // fails.
    // static_assert(FpType<i64>);
    // ```
    template <typename T>
    concept FpType = std::floating_point<T>;
} // namespace rng::num

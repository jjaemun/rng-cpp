#pragma once


#include <algorithm>
#include <array>
#include <bit>
#include <limits>
#include <optional>
#include <ranges>
#include <span>


#include "rng/types.hpp"


namespace rng::rngs {
    // `xoshiro256++` pseudorandom generator.
    class Xoshiro256PlusPlus final {
        
        std::array<u64, 4> state;

    private:
        explicit Xoshiro256PlusPlus(const std::array<u64, 4>& state_) noexcept 
                : state(state_) {}

    public:
        [[nodiscard]]
        static auto seed_from_state(const std::array<u64, 4>& words) noexcept
            -> std::optional<Xoshiro256PlusPlus>
        {
            for (auto word : words)
                if (word != 0)
                    return Xoshiro256PlusPlus{words};

            return std::nullopt;
        }

        [[nodiscard]]
        static auto seed_from_raw_bytes(std::span<const u8> bytes) noexcept
            -> std::optional<Xoshiro256PlusPlus>
        {
            if (bytes.size() != 32)
                return std::nullopt;

            bool nonzero{false};
            std::array<u64, 4> words{};

            for (auto offset = 0; offset < bytes.size(); ++offset) {
                nonzero |= bytes[offset] != 0;

                const auto word = offset / sizeof(u64); 
                const auto byte = offset % sizeof(u64); 

                words[word] |= 
                        static_cast<u64>(bytes[offset]) << (8u * byte);
            }
            
            if (nonzero)
                return Xoshiro256PlusPlus{words};

            return seed_from_u64(0);
        }

        [[nodiscard]]
        static Xoshiro256PlusPlus seed_from_u64(u64 seed) noexcept {
            std::array<u64, 4u> words{};
            for (auto& word : words) {
                seed += 0x9e3779b97f4a7c15ULL;

                auto copy = seed;
                copy = (copy ^ (copy >> 30)) * 0xbf58476d1ce4e5b9ULL;
                copy = (copy ^ (copy >> 27)) * 0x94d049bb133111ebULL;
                word = copy ^ (copy >> 31);
            }
        
            return Xoshiro256PlusPlus{words};
        }

        using result_type = u64;

        [[nodiscard]]
        static constexpr u64 min() noexcept {
            return 0u;
        }

        [[nodiscard]]
        static constexpr u64 max() noexcept {
            return std::numeric_limits<u64>::max();
        }

        [[nodiscard]]
        u64 operator()() noexcept {
            return next_u64();
        }
 
        void discard(u64 n) noexcept {
            for (; n > 0; --n)
                (void)(next_u64());
        }

        [[nodiscard]]
        u32 next_u32() noexcept {
            return static_cast<u32>(next_u64() >> 32);
        }

        [[nodiscard]]
        u64 next_u64() noexcept {
            const u64 result =
                std::rotl(state[0] + state[3], 23) + state[0];

            const u64 t = state[1] << 17;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= t;
            state[3] = std::rotl(state[3], 45);

            return result;
        }

        void fill_bytes(std::span<std::byte> dst) noexcept {
            while (dst.size() >= sizeof(u64)) {
                const u64 word = next_u64();

                for (auto i = 0; i < sizeof(u64); ++i) {
                    dst[i] = static_cast<std::byte>(word >> (8 * i));
                }

                dst = dst.subspan(sizeof(u64));
            }
        
            if (!dst.empty()) {
                const u64 word = next_u64();

                for (auto i = 0; i < dst.size(); ++i) {
                    dst[i] = static_cast<std::byte>(word >> (8 * i));
                } 
            }
        }
   };
} // namespace rng::rngs

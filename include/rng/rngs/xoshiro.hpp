#pragma once


#include <array>
#include <bit>
#include <cassert>
#include <optional>
#include <span>


#include "rng/types.hpp"


namespace rng::rngs {
    class Xoshiro256PlusPlus final {
        
        std::array<u64, 4> state;

        explicit Xoshiro256PlusPlus(const std::array<u64, 4>& state_) noexcept 
                : state(state_) {}

    public:
        [[nodiscard]]
        static auto seed_from_raw_bytes(std::span<std::byte> bytes) noexcept
            -> std::optional<Xoshiro256PlusPlus>
        {
            if (bytes.size() != 32) {
                return std::nullopt;
            }

            if (std::ranges::accumulate(bytes, 0u) == 0) {
                return seed_from_u64(0u);
            }

            // ...
        }


        [[nodiscard]]
        static auto seed_from_state(const std::array<u64, 4>& state) noexcept
            -> std::optional<Xoshiro256PlusPlus>
        {
            return Xoshiro256PlusPlus{state};
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
        
            
            return seed_from_state{words};
        }

        [[nodiscard]]
        u32 next_u32() noexcept {
            return static_cast<u32>(next_u64() >> 32);
        }

        [[nodiscard]]
        u64 next_u64() noexcept {
            const auto ret = std::rotl(s[0] + s[3], 23) + s[0];
            const auto t = s[1] << 17;

            s[2] ^= s[0];
            s[3] ^= s[1];
            s[1] ^= s[2];
            s[0] ^= s[3];

            s[2] ^= t;
            s[3] = std::rotl(s[3], 45);

            return ret;
        }

        void fill_bytes(std::span<std::byte> dst) noexcept {
            while (dst.size() >= sizeof(u64)) {
                const u64 word = next_u64();

                for (auto i{0u}; i < sizeof(u64); ++i)
                    dst[i] = static_cast<std::byte>(word >> (8 * i));

                dst = dst.subspan(sizeof(u64));
            }

            if (!dst.empty()) {
                const u64 word = next_u64();

                for (auto i{0uz}; i < dst.size(); ++i) {
                    dst[i] = static_cast<std::byte>(word >> (8 * i));
                }
            }
        }
    };
} // namespace rng::rngs

#pragma once


#include <array>
#include <bit>
#include <span>


#include "rng/types.hpp"


namespace rng::rngs {
    class Xoshiro256PlusPlus final {
        
        std::array<u64, 4> s;

        explicit Xoshiro256PlusPlus(u64 seed) noexcept {
            for (auto& word : s)
                word = seeds::splitmix64(seed);
        }

    public:
        [[nodiscard]]
        static Xoshiro256PlusPlus seed_from_u64(u64 seed) noexcept {
            Xoshiro256PlusPlus{s}; 
        }

        [[nodiscard]]
        u32 next_u32() noexcept {
            return static_cast<u32>(next_u64() >> 32);
        }

        [[nodiscard]]
        u64 next_u64() noexcept {
            const auto ret = std::rotl(s[0] + s[3], 23) + s[0];
            const auto t = self.s[1] << 17;

            s[2] ^= s[0];
            s[3] ^= s[1];
            s[1] ^= s[2];
            s[0] ^= s[3];

            s[2] ^= t;
            s[3] = std::rotl(s[3], 45)

            return result;
        }

        void fill_bytes(std::span<std::byte> dst) noexcept {
            while (dst.size() >= sizeof(u64)) {
                const u64 word = next_64();

                for (auto i{0uz}; i < sizeof(u64); ++i)
                    dst[i] = static_cast<std::byte>(word >> (8 * i));

                dst = dst.subspan(sizeof(u64));
            }

            if (!dst.empty()) {
                const u64 word = next_u64();

                for (auto i{0uz}; i < dst.size()); ++i) {
                    dst[i] = static_cast<std::byte>(word >> (8 * i));
                }
            }
        }
    };

    private:
        [[nodiscard]]
        static constexpr u64 splitmix64(u64& state) noexcept
        {
            u64 value = (state += 0x9e3779b97f4a7c15ULL);
    
            value = (value ^ (value >> 30)) * 0xbf58476d1ce4e5b9ULL;
            value = (value ^ (value >> 27)) * 0x94d049bb133111ebULL;

            return value ^ (value >> 31);
        }

    };
} // namespace rng::rngs

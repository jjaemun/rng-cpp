#pragma once


#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <optional>
#include <ranges>
#include <span>


#include "rng/types.hpp"


namespace rng::rngs {
    class Xoshiro256PlusPlus final {
        
        std::array<u64, 4> state;

    private:
        explicit Xoshiro256PlusPlus(const std::array<u64, 4>& state_) noexcept 
                : state(state_) {}

    public:
        [[nodiscard]]
        static auto seed_from_state(const std::array<u64, 4>& state) noexcept
            -> std::optional<Xoshiro256PlusPlus>
        {
            const auto zeroed = [](u64 word) noexcept {
                return word == u64{0};
            }
    
            if (std::ranges::all_of(state, zeroed))
                return std::nullopt;

            return Xoshiro256PlusPlus{state};
        }

        [[nodiscard]]
        static auto seed_from_raw_bytes(std::span<const std::byte> bytes) noexcept
            -> std::optional<Xoshiro256PlusPlus>
        {
            if (bytes.size() != 32)
                return std::nullopt;

            const auto zeroed = [](std::byte byte) noexcept {
                return byte == std::byte{0u};
            };

            if (std::ranges::all_of(bytes, zeroed))
                return seed_from_u64(0u);
            

            std::array<u64, 4> state{}; 
            for (auto word : std::views::iota(0u, state.size())) {
                for (auto byte : std::views::iota(0u, sizeof(u64))) {
                    const auto offset = word * sizeof(u64) + byte;
                    
                    state[word] |= 
                        static_cast<u64>(
                            std::to_integer<u8>(bytes[offset])
                        ) << (8u * byte);
                }
            }

            return seed_from_state(state);
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
        static constexpr auto min() noexcept {
            return 0u;
        }

        [[nodiscard]]
        static constexpr auto max() noexcept {
            return num::MAX<u64>();
        }

        [[nodiscard]]
        auto operator()() noexcept {
            return next_u64();
        }
 
        [[nodiscard]]
        u32 next_u32() noexcept {
            return static_cast<u32>(next_u64() >> 32);
        }

        [[nodiscard]]
        u64 next_u64() noexcept {
            const u64 result =
            std::rotl(state[0] + state[3], 23) + state[0];

            const u64 temporary = state[1] << 17;

            state[2] ^= state[0];
            state[3] ^= state[1];
            state[1] ^= state[2];
            state[0] ^= state[3];

            state[2] ^= temporary;
            state[3] = std::rotl(state[3], 45);

            return result;
        }

        void fill_bytes(std::span<std::byte> dst) noexcept {
            while (dst.size() >= sizeof(u64)) {
                const u64 word = next_u64();

                for (auto i : std::views::iota(0u, sizeof(u64)))
                    dst[i] = static_cast<std::byte>(word >> (8 * i));
                

                dst = dst.subspan(sizeof(u64));
            }
        
            if (!dst.empty()) {
                const u64 word = next_u64();

                for (auto : std::views::iota(0u, dst.size())
                    dst[i] = static_cast<std::byte>(word >> (8 * i));
                
            }
        }
   };
} // namespace rng::rngs

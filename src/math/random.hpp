// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_MATH_RANDOM_HPP
#define HEADER_PINGUS_MATH_RANDOM_HPP

#include <cstdint>
#include <random>
#include <string_view>

namespace pingus {

/** Seedable random number generator with output that is identical on
    all platforms. std::mt19937 itself is fully specified, the
    std::uniform_*_distribution classes are not, so the range mapping is
    done by hand. */
class Random
{
public:
  /** Derive a seed from a string, e.g. a level checksum (FNV-1a) */
  static uint32_t seed_from_string(std::string_view text)
  {
    uint32_t hash = 2166136261u;
    for (char c : text) {
      hash ^= static_cast<uint8_t>(c);
      hash *= 16777619u;
    }
    return hash;
  }

public:
  explicit Random(uint32_t seed = 0) : m_engine(seed) {}

  void seed(uint32_t seed) { m_engine.seed(seed); }

  /** Random integer in [0, bound), returns 0 if bound <= 0 */
  int next_int(int bound)
  {
    if (bound <= 0) {
      return 0;
    }
    return static_cast<int>(m_engine() % static_cast<uint32_t>(bound));
  }

  /** Random float in [0, 1) */
  float next_float()
  {
    return static_cast<float>(m_engine() >> 8) * (1.0f / 16777216.0f);
  }

private:
  std::mt19937 m_engine;
};

} // namespace pingus

#endif

/* EOF */

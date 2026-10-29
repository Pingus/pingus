// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef HEADER_PINGUS_PINGUS_STATE_HASH_HPP
#define HEADER_PINGUS_PINGUS_STATE_HASH_HPP

#include <cstdint>
#include <ostream>

namespace pingus {

class Server;
class World;

/** FNV-1a hash over values fed in a platform independent byte order.
    Floats are hashed by their bit pattern, so any difference, even in
    the last bit, shows up. */
class StateHash
{
public:
  StateHash() : m_hash(14695981039346656037ull) {}

  void add_u8(uint8_t value)
  {
    m_hash ^= value;
    m_hash *= 1099511628211ull;
  }

  void add_u32(uint32_t value);
  void add_u64(uint64_t value);
  void add_int(int value) { add_u32(static_cast<uint32_t>(value)); }
  void add_float(float value);

  uint64_t get() const { return m_hash; }

private:
  uint64_t m_hash;
};

/** Hash of the gameplay state that is cheap to compute: time, the
    gameplay random number generator, the action pool and the live
    pingus. Does not include the terrain, see hash_terrain(). Internal
    state of pingu actions and traps is not covered directly, it only
    shows up once it changes a pingu. */
uint64_t hash_game_state(Server& server);

/** Hash of the collision map, expensive on large levels */
uint64_t hash_terrain(World& world);

/** Write the state of every live pingu, one per line, for locating
    where two runs diverge. Floats are written in the shortest form
    that reads back to the same value. */
void write_pingu_states(std::ostream& out, World& world);

} // namespace pingus

#endif

/* EOF */

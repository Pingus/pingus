// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/state_hash.hpp"

#include <bit>
#include <charconv>
#include <string_view>

#include "pingus/action_holder.hpp"
#include "pingus/collision_map.hpp"
#include "pingus/pingu.hpp"
#include "pingus/pingu_holder.hpp"
#include "pingus/server.hpp"
#include "pingus/world.hpp"

namespace pingus {

namespace {

void hash_pingu(StateHash& hash, Pingu& pingu)
{
  Vector2f const pos = pingu.get_pos();
  glm::vec2 const velocity = pingu.get_velocity();

  hash.add_u32(pingu.get_id());
  hash.add_int(pingu.get_owner());
  hash.add_int(pingu.get_status());
  hash.add_int(pingu.get_action());
  hash.add_int(pingu.get_previous_action());
  hash.add_int(pingu.get_action_time());
  hash.add_int(pingu.direction());
  hash.add_float(pos.x());
  hash.add_float(pos.y());
  hash.add_float(velocity.x);
  hash.add_float(velocity.y);
}

std::string_view float_to_chars(char* buffer, size_t len, float value)
{
  auto const result = std::to_chars(buffer, buffer + len, value);
  return std::string_view(buffer, static_cast<size_t>(result.ptr - buffer));
}

} // namespace

void
StateHash::add_u32(uint32_t value)
{
  for (int i = 0; i < 4; ++i) {
    add_u8(static_cast<uint8_t>(value >> (8 * i)));
  }
}

void
StateHash::add_u64(uint64_t value)
{
  for (int i = 0; i < 8; ++i) {
    add_u8(static_cast<uint8_t>(value >> (8 * i)));
  }
}

void
StateHash::add_float(float value)
{
  add_u32(std::bit_cast<uint32_t>(value));
}

uint64_t hash_game_state(Server& server)
{
  World& world = *server.get_world();
  StateHash hash;

  hash.add_int(world.get_time());
  hash.add_u32(world.get_game_random().peek());

  ActionHolder& actions = *server.get_action_holder();
  for (ActionName::Enum const action : actions.get_available_actions()) {
    hash.add_int(action);
    hash.add_int(actions.get_available(action));
  }

  PinguHolder& pingus = *world.get_pingus();
  hash.add_int(pingus.get_number_of_released());
  hash.add_int(pingus.get_number_of_exited());
  hash.add_int(pingus.get_number_of_killed());
  for (unsigned int id = 0; id < pingus.get_end_id(); ++id) {
    if (Pingu* pingu = pingus.get_pingu(id)) {
      hash_pingu(hash, *pingu);
    }
  }

  return hash.get();
}

uint64_t hash_terrain(World& world)
{
  CollisionMap& colmap = *world.get_colmap();
  StateHash hash;
  hash.add_int(colmap.get_width());
  hash.add_int(colmap.get_height());
  unsigned char const* data = colmap.get_data();
  size_t const size = static_cast<size_t>(colmap.get_width()) * static_cast<size_t>(colmap.get_height());
  for (size_t i = 0; i < size; ++i) {
    hash.add_u8(data[i]);
  }
  return hash.get();
}

void write_pingu_states(std::ostream& out, World& world)
{
  PinguHolder& pingus = *world.get_pingus();
  char buf[4][32];
  for (unsigned int id = 0; id < pingus.get_end_id(); ++id) {
    if (Pingu* pingu = pingus.get_pingu(id)) {
      Vector2f const pos = pingu->get_pos();
      glm::vec2 const velocity = pingu->get_velocity();
      out << "  pingu " << id
          << " " << ActionName::to_string(pingu->get_action())
          << " pos=" << float_to_chars(buf[0], sizeof(buf[0]), pos.x())
          << "," << float_to_chars(buf[1], sizeof(buf[1]), pos.y())
          << " vel=" << float_to_chars(buf[2], sizeof(buf[2]), velocity.x)
          << "," << float_to_chars(buf[3], sizeof(buf[3]), velocity.y)
          << " dir=" << pingu->direction()
          << " action-time=" << pingu->get_action_time()
          << "\n";
    }
  }
}

} // namespace pingus

/* EOF */

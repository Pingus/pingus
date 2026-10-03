// SPDX-FileCopyrightText: 2026 Ingo Ruhnke <grumbel@gmail.com>
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pingus/animation_clock.hpp"

#include <logmich/log.hpp>

#include "engine/display/sprite.hpp"
#include "engine/display/sprite_description.hpp"
#include "pingus/resource.hpp"

namespace pingus {

AnimationClock
AnimationClock::from_sprite(std::string const& res_name)
{
  SpriteDescription* desc = Resource::load_sprite_desc(res_name);
  if (desc)
  {
    return from_description(*desc);
  }
  else
  {
    // Sprite falls back to a static 404 image in this case
    log_error("{}: failed to load sprite description, using static clock", res_name);
    return AnimationClock();
  }
}

AnimationClock
AnimationClock::from_description(SpriteDescription const& desc)
{
  return AnimationClock(desc.speed, desc.array.width() * desc.array.height(), desc.loop);
}

AnimationClock::AnimationClock() :
  AnimationClock(0, 1, true)
{
}

AnimationClock::AnimationClock(int frame_delay, int frame_count, bool loop) :
  m_frame_delay(frame_delay),
  m_frame_count(frame_count),
  m_loop(loop),
  m_finished(false),
  m_frame(0),
  m_tick_count(0)
{
}

void
AnimationClock::update()
{
  // Mirrors SpriteImpl::update()
  if (m_finished || m_frame_delay == 0)
    return;

  int const total_time = m_frame_delay * m_frame_count;
  m_tick_count += kTickMillis;
  if (m_tick_count >= total_time)
  {
    if (m_loop)
    {
      m_tick_count = m_tick_count % total_time;
      m_frame = m_tick_count / m_frame_delay;
    }
    else
    {
      m_finished = true;
    }
  }
  else
  {
    m_frame = m_tick_count / m_frame_delay;
  }
}

void
AnimationClock::restart()
{
  m_finished = false;
  m_frame = 0;
  m_tick_count = 0;
}

void
AnimationClock::finish()
{
  m_finished = true;
}

float
AnimationClock::progress() const
{
  if (m_frame_count == 0)
    return 0.0f;

  return static_cast<float>(m_frame) / static_cast<float>(m_frame_count);
}

void
AnimationClock::apply_to(Sprite& sprite) const
{
  sprite.set_frame(m_frame);
}

} // namespace pingus

/* EOF */

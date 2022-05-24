// Pingus - A free Lemmings clone
// Copyright (C) 2000 Ingo Ruhnke <grumbel@gmail.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "engine/sound/sound_real.hpp"

#include <SDL.h>
#include <stdexcept>

#include <logmich/log.hpp>
#include <wstsound/sound_source.hpp>
#include <wstsound/openal_system.hpp>
#include <wstsound/sound_manager.hpp>

#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "util/raise_exception.hpp"

namespace pingus::sound {

PingusSoundReal::PingusSoundReal() :
  m_sound_manager(),
  m_music_source(),
  m_music_volume(1.0f),
  m_sound_volume(1.0f),
  m_master_volume(1.0f)
{
  m_sound_manager = std::make_unique<wstsound::SoundManager>();
}

PingusSoundReal::~PingusSoundReal()
{
  real_stop_music();
}

void
PingusSoundReal::update(float delta)
{
  m_sound_manager->update(delta);
}

void
PingusSoundReal::real_play_sound(const std::string& name, float volume, float panning)
{
  if (globals::sound_enabled &&
      m_master_volume > 0 &&
      m_sound_volume > 0)
  {
    std::filesystem::path filename = g_path_manager.complete("sounds/" + name + ".wav");
    wstsound::SoundSourcePtr source = m_sound_manager->sound().prepare(filename, wstsound::SoundSourceType::STREAM);
    // if (!chunk)
    // {
    //   log_error("Can't open sound '{}' -- skipping\n  Mix_Error: {}", name, Mix_GetError());
    //   return;
    // }

    source->set_position(panning, 0.0f, 0.0f);
    source->set_gain(volume * m_music_volume * m_master_volume);
    source->play();
    m_sound_manager->manage(std::move(source));
  }
}

void
PingusSoundReal::real_stop_music ()
{
  if (m_music_source)
  {
    m_music_source->finish();
    m_music_source = {};
  }
}

void
PingusSoundReal::real_play_music(const std::string& filename, float volume, bool loop)
{
  if (globals::music_enabled &&
      m_master_volume > 0 &&
      m_music_volume > 0)
  {
    log_info("PingusSoundReal: Playing music: {}", filename);

    real_stop_music();

    m_music_source = m_sound_manager->music().prepare(filename, wstsound::SoundSourceType::STREAM);
    // if (!music_sample)
    // {
    //   log_error("Can't load music: {}' -- skipping\n  Mix_Error: {}", filename, Mix_GetError());
    //   return;
    // }
    m_music_source->set_looping(loop);
    m_music_source->set_gain(volume * m_music_volume * m_master_volume);
    m_music_source->play();
  }
}

void
PingusSoundReal::set_sound_volume(float volume)
{
  m_sound_volume = volume;
  apply_volume_changes();
}

void
PingusSoundReal::set_music_volume(float volume)
{
  m_music_volume = volume;
  apply_volume_changes();
}

void
PingusSoundReal::set_master_volume(float volume)
{
  m_master_volume = volume;
  apply_volume_changes();

}

float
PingusSoundReal::get_sound_volume() const
{
  return m_sound_volume;
}

float
PingusSoundReal::get_music_volume() const
{
  return m_music_volume;
}

float
PingusSoundReal::get_master_volume() const
{
  return m_master_volume;
}

void
PingusSoundReal::apply_volume_changes() // NOLINT
{
  m_sound_manager->sound().set_gain(m_sound_volume * m_master_volume);
  m_sound_manager->music().set_gain(m_music_volume * m_master_volume);
}

} // namespace pingus::sound

/* EOF */


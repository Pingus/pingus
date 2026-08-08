/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2005 Matthias Braun <matze@braunis.de>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "openal_sound_source.hpp"

#include <assert.h>
#include <iostream>

#define AL_ALEXT_PROTOTYPES
#include <efx.h>

#include "effect_slot.hpp"
#include "filter.hpp"
#include "sound_error.hpp"
#include "sound_manager.hpp"

namespace wstsound {

OpenALSoundSource::OpenALSoundSource(SoundChannel& channel) :
  m_channel(channel),
  m_source(),
  m_gain(1.0f),
  m_direct_filter(),
  m_filter(),
  m_effect_slot()
{
  alGenSources(1, &m_source);

  // Don't catch anything here: force the caller to catch the error, so that
  // the caller won't handle an object in an invalid state thinking it's clean
  OpenALSystem::check_al_error("Couldn't create audio source: ");

  set_reference_distance(128);
}

OpenALSoundSource::~OpenALSoundSource()
{
  finish();
  alDeleteSources(1, &m_source);
  OpenALSystem::warn_al_error("Couldn't delete source: ");
}

void
OpenALSoundSource::play()
{
  alSourcePlay(m_source);
  OpenALSystem::warn_al_error("Couldn't start audio source: ");
}

void
OpenALSoundSource::pause()
{
  alSourcePause(m_source);
  OpenALSystem::warn_al_error("Couldn't pause audio source: ");
}

void
OpenALSoundSource::finish()
{
  alSourceStop(m_source);
  OpenALSystem::warn_al_error("Problem stopping audio source: ");
}

SourceState
OpenALSoundSource::get_state() const
{
  ALint state = AL_STOPPED;
  alGetSourcei(m_source, AL_SOURCE_STATE, &state);
  OpenALSystem::warn_al_error("OpenALSoundSource::get_state(): ");

  switch (state)
  {
    case AL_INITIAL:
      return SourceState::Paused;

    case AL_PLAYING:
      return SourceState::Playing;

    case AL_PAUSED:
      return SourceState::Paused;

    case AL_STOPPED:
      return SourceState::Finished;

    default:
      return SourceState::Finished;
  }
}

void
OpenALSoundSource::seek_to(float sec)
{
  alSourcef(m_source, AL_SEC_OFFSET, sec);
  OpenALSystem::warn_al_error("OpenALSoundSource::seek_to: ");
}

void
OpenALSoundSource::seek_to_sample(int sample)
{
  alSourcei(m_source, AL_SAMPLE_OFFSET, sample);
  OpenALSystem::warn_al_error("OpenALSoundSource::seek_to_sample: ");
}

float
OpenALSoundSource::get_pos() const
{
  float sec = 0.0f;
  alGetSourcef(m_source, AL_SEC_OFFSET, &sec);
  OpenALSystem::warn_al_error("OpenALSoundSource::get_pos: ");
  return sec;
}

int
OpenALSoundSource::get_sample_pos() const
{
  ALint sample_pos;
  alGetSourcei(m_source, AL_SAMPLE_OFFSET, &sample_pos);
  OpenALSystem::warn_al_error("OpenALSoundSource::get_sample_pos: ");
  return sample_pos;
}

void
OpenALSoundSource::set_looping(bool looping)
{
  alSourcei(m_source, AL_LOOPING, looping ? AL_TRUE : AL_FALSE);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_looping: ");
}

void
OpenALSoundSource::set_loop(int sample_beg, int sample_end)
{
  throw SoundError("OpenALSoundSource::set_loop() not supported for non-streaming sources");
}

void
OpenALSoundSource::set_relative(bool relative)
{
  alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_relative: ");
}

void
OpenALSoundSource::set_position(float x, float y, float z)
{
  alSource3f(m_source, AL_POSITION, x, y, z);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_position: ");
}

void
OpenALSoundSource::set_velocity(float x, float y, float z)
{
  alSource3f(m_source, AL_VELOCITY, x, y, z);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_velocity: ");
}

void
OpenALSoundSource::set_gain(float gain)
{
  m_gain = gain;
  update_gain();
}

float
OpenALSoundSource::get_gain() const
{
  return m_gain;
}

void
OpenALSoundSource::set_pitch(float pitch)
{
  alSourcef(m_source, AL_PITCH, pitch);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_pitch: ");
}

void
OpenALSoundSource::set_reference_distance(float distance)
{
  alSourcef(m_source, AL_REFERENCE_DISTANCE, distance);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_reference_distance: ");
}

void
OpenALSoundSource::set_rolloff_factor(float factor)
{
  alSourcef(m_source, AL_ROLLOFF_FACTOR, factor);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_rolloff_factor: ");
}

void
OpenALSoundSource::update_gain() const
{
  alSourcef(m_source, AL_GAIN, m_channel.get_gain() * get_gain() * m_fade_gain);
  OpenALSystem::warn_al_error("OpenALSoundSource::update_gain: ");
}

void
OpenALSoundSource::update(float delta)
{
  SoundSource::update(delta);
}

void
OpenALSoundSource::set_direct_filter(FilterPtr const& filter)
{
  m_direct_filter = filter;

  if (!filter) {
    alSourcei(m_source, AL_DIRECT_FILTER, AL_FILTER_NULL);
  } else {
    alSourcei(m_source, AL_DIRECT_FILTER, filter->handle());
  }
  OpenALSystem::warn_al_error("OpenALSoundSource::set_direct_filter: ");
}

void
OpenALSoundSource::set_effect_slot(EffectSlotPtr const& slot, FilterPtr const& filter)
{
  m_effect_slot = slot;
  m_filter = filter;
  ALint const auxiliary_send = 0; // can have more than one!

  alSource3i(m_source, AL_AUXILIARY_SEND_FILTER,
             slot->handle(),
             auxiliary_send,
             filter ? m_filter->handle() : AL_FILTER_NULL);
  OpenALSystem::warn_al_error("OpenALSoundSource::set_effect_slot: ");
}

} // namespace wstsound

/* EOF */

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

#include <assert.h>
#include <filesystem>
#include <iostream>
#include <sstream>

#include "openal_buffer.hpp"
#include "dummy_sound_source.hpp"
#include "effect.hpp"
#include "effect_slot.hpp"
#include "filter.hpp"
#include "openal_system.hpp"
#include "sound_error.hpp"
#include "sound_file.hpp"
#include "sound_manager.hpp"
#include "sound_source_type.hpp"
#include "static_sound_source.hpp"
#include "stream_sound_source.hpp"

namespace wstsound {

SoundManager::SoundManager(std::unique_ptr<OpenALSystem> openal,
                           OpenFunc open_func) :
  m_openal(std::move(openal)),
  m_open_func(std::move(open_func)),
  m_listener(*this),
  m_channels(),
  m_buffer_cache(),
  m_managed_sources()
{
  m_channels.emplace_back(std::make_unique<SoundChannel>(*this));
  m_channels.emplace_back(std::make_unique<SoundChannel>(*this));
  m_channels.emplace_back(std::make_unique<SoundChannel>(*this));
}

SoundManager::SoundManager(OpenFunc open_func) :
  m_openal(),
  m_open_func(std::move(open_func)),
  m_listener(*this),
  m_channels(),
  m_buffer_cache(),
  m_managed_sources()
{
  m_channels.emplace_back(std::make_unique<SoundChannel>(*this));
  m_channels.emplace_back(std::make_unique<SoundChannel>(*this));
  m_channels.emplace_back(std::make_unique<SoundChannel>(*this));

  m_openal = std::make_unique<OpenALSystem>();
  try {
    m_openal->open_real_device();
  } catch(std::exception& err) {
    std::cerr << "Couldn't initialize audio device:" << err.what() << "\n";
    std::cerr << "Disabling sound\n";
    m_openal.reset();
  }
}

SoundManager::~SoundManager()
{
}

OpenALBufferPtr
SoundManager::load_file_into_buffer(std::unique_ptr<SoundFile> file)
{
  std::vector<char> samples(file->get_size());
  size_t total_bytesread = 0;
  while (true) {
    size_t bytesread = file->read(samples.data() + total_bytesread, 1024 * 64);
    if (bytesread == 0) {
      break;
    }
    total_bytesread += bytesread;

    assert(total_bytesread <= file->get_size());
  }

  return m_openal->create_buffer(file->get_format().get_openal_format(),
                                 samples.data(),
                                 static_cast<ALsizei>(total_bytesread),
                                 file->get_format().get_rate());
}

std::unique_ptr<SoundFile>
SoundManager::load_sound_file(std::filesystem::path const& filename)
{
  if (m_open_func) {
    auto is = m_open_func(filename);
    return SoundFile::from_stream(std::move(is));
  } else {
    return SoundFile::from_file(filename);
  }
}

void
SoundManager::preload(std::filesystem::path const& filename)
{
  if (!m_openal) { return; }

  auto it = m_buffer_cache.find(filename);
  if (it == m_buffer_cache.end())
  {
    OpenALBufferPtr buffer = load_file_into_buffer(load_sound_file(filename));
    m_buffer_cache.insert(std::make_pair(filename, std::move(buffer)));
  }
}

SoundSourcePtr
SoundManager::create_sound_source(std::unique_ptr<SoundFile> sound_file,
                                  SoundChannel& channel,
                                  SoundSourceType type)
{
  if (!m_openal) {
    return SoundSourcePtr(new DummySoundSource());
  }

  switch(type)
  {
    case SoundSourceType::STATIC:
      return SoundSourcePtr(new StaticSoundSource(channel,
                                                  load_file_into_buffer(std::move(sound_file))));

    case SoundSourceType::STREAM:
      return SoundSourcePtr(new StreamSoundSource(channel, std::move(sound_file)));
  }

  throw std::invalid_argument("invalid SoundSourceType");
}

SoundSourcePtr
SoundManager::create_sound_source(std::filesystem::path const& filename, SoundChannel& channel,
                                  SoundSourceType type)
{
  if (!m_openal) {
    return SoundSourcePtr(new DummySoundSource);
  }

  switch(type)
  {
    case SoundSourceType::STATIC:
      {
        OpenALBufferPtr buffer;

        // reuse an existing static sound buffer
        auto it = m_buffer_cache.find(filename);
        if (it != m_buffer_cache.end()) {
          buffer = it->second;
        } else {
          buffer = load_file_into_buffer(load_sound_file(filename));
          m_buffer_cache.insert(std::make_pair(filename, buffer));
        }

        return SoundSourcePtr(new StaticSoundSource(channel, buffer));
      }
      break;

    case SoundSourceType::STREAM:
      {
        std::unique_ptr<SoundFile> sound_file = load_sound_file(filename);
        return SoundSourcePtr(new StreamSoundSource(channel, std::move(sound_file)));
      }
      break;
  }

  throw std::invalid_argument("invalid SoundSourceType");
}

void
SoundManager::set_gain(float gain)
{
  if (!m_openal) { return; }
  alListenerf(AL_GAIN, gain);
}

void
SoundManager::manage(SoundSourcePtr source)
{
  m_managed_sources.emplace_back(std::move(source));
}

void
SoundManager::update(float delta)
{
  std::erase_if(m_managed_sources,
                [](SoundSourcePtr& source) {
                  return source->get_state() == SourceState::Finished;
                });

  for(std::unique_ptr<SoundChannel>& channel : m_channels) {
    channel->update(delta);
  }

  if (m_openal) {
    m_openal->update();
  }
}

EffectSlotPtr
SoundManager::create_effect_slot()
{
  return std::make_shared<EffectSlot>();
}

EffectPtr
SoundManager::create_effect(ALuint effect_type)
{
  return std::make_shared<Effect>(effect_type);
}

FilterPtr
SoundManager::create_filter(ALuint filter_type)
{
  return std::make_shared<Filter>(filter_type);
}

} // namespace wstsound

/* EOF */

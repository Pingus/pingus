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

#include "stream_sound_source.hpp"

#include <array>
#include <iostream>
#include <stdexcept>

#include "sound_manager.hpp"
#include "sound_file.hpp"

namespace wstsound {

StreamSoundSource::StreamSoundSource(SoundChannel& channel, std::unique_ptr<SoundFile> sound_file) :
  OpenALSoundSource(channel),
  m_sound_file(std::move(sound_file)),
  m_buffers(),
  m_buffers_queued(false),
  m_format(m_sound_file->get_format().get_openal_format()),
  m_total_samples_processed(0),
  m_state(SourceState::Paused),
  m_loop()
{
  alGenBuffers(static_cast<ALsizei>(m_buffers.size()), m_buffers.data());
  OpenALSystem::check_al_error("Couldn't allocate audio buffers: ");
}

StreamSoundSource::~StreamSoundSource()
{
  clear_queue();

  alDeleteBuffers(static_cast<ALsizei>(m_buffers.size()), m_buffers.data());
  OpenALSystem::warn_al_error("Couldn't delete audio buffers: ");
}

void
StreamSoundSource::set_looping(bool looping)
{
  // Native OpenAL looping will result in only the queue being looped, not
  // the whole song as provided by the SoundFile, so we do it manually.
  if (looping) {
    m_loop = Loop{0, m_sound_file->get_sample_duration()};
  } else {
    m_loop = std::nullopt;
  }
}

void
StreamSoundSource::set_loop(int sample_beg, int sample_end)
{
  // FIXME: should be handle loops that circle around the end?
  m_loop = Loop{
    std::max(sample_beg, 0),
    std::min(sample_end, m_sound_file->get_sample_duration())
  };

  if (sample_beg > sample_end) {
    throw std::invalid_argument("StreamSoundSource::set_loop(): invalid loop range");
  }
}

void
StreamSoundSource::seek_to_sample(int sample)
{
  clear_queue();

  m_sound_file->seek_to_sample(sample);
  m_total_samples_processed = sample;
}

void
StreamSoundSource::seek_to(float sec)
{
  seek_to_sample(sec_to_sample(sec));
}

float
StreamSoundSource::get_pos() const
{
  return sample_to_sec(get_sample_pos());
}

int
StreamSoundSource::get_sample_pos() const
{
  ALint sample_offset;
  alGetSourcei(m_source, AL_SAMPLE_OFFSET, &sample_offset);

  return (m_total_samples_processed + sample_offset);
}

int
StreamSoundSource::get_sample_duration() const
{
  return m_sound_file->get_sample_duration();
}

float
StreamSoundSource::get_duration() const
{
  return m_sound_file->get_duration();
}

void
StreamSoundSource::play()
{
  if (m_state == SourceState::Playing) { return; }

  m_state = SourceState::Playing;

  update_queue();
  OpenALSoundSource::play();
}

void
StreamSoundSource::finish()
{
  if (m_state == SourceState::Finished) { return; }

  m_state = SourceState::Finished;

  clear_queue();
  OpenALSoundSource::finish();
}

void
StreamSoundSource::pause()
{
  if (m_state == SourceState::Paused) { return; }

  m_state = SourceState::Paused;

  OpenALSoundSource::pause();
}

void
StreamSoundSource::update(float delta)
{
  OpenALSoundSource::update(delta);

  if (m_state == SourceState::Playing)
  {
    update_queue();

    ALint queued_buffers = 0;
    alGetSourcei(m_source, AL_BUFFERS_QUEUED, &queued_buffers);
    if (queued_buffers == 0)
    {
      // refilling didn't lead to anything getting queued, thus the
      // SoundFile must have hit EOF, stop the source
      m_state = SourceState::Finished;
      OpenALSoundSource::finish();
    }
    else
    {
      ALint state = AL_STOPPED;
      alGetSourcei(m_source, AL_SOURCE_STATE, &state);
      // Source is stopped, but should still be plalying, thus a
      // buffer underrun occured, restart the source.
      if (state == AL_STOPPED)
      {
        std::cerr << "Restarting audio source because of buffer underrun.\n";
        OpenALSoundSource::play();
      }
    }
  }
}

float
StreamSoundSource::sample_to_sec(int sample) const
{
  return static_cast<float>(sample) / static_cast<float>(m_sound_file->get_format().get_rate());
}

int
StreamSoundSource::sec_to_sample(float sec) const
{
  return static_cast<int>(sec * static_cast<float>(m_sound_file->get_format().get_rate()));
}

void
StreamSoundSource::fill_buffer_and_queue(ALuint buffer)
{
  std::array<char, STREAMFRAGMENTSIZE> bufferdata;
  size_t total_bytesread = 0;

  // fill buffer with data from m_sound_file
  do {
    size_t bytesrequested = STREAMFRAGMENTSIZE - total_bytesread;

    if (m_loop) {
      bytesrequested = std::min(m_sound_file->get_format().sample2bytes(m_loop->sample_end) - m_sound_file->tell(),
                                bytesrequested);
    }

    size_t const bytesread = m_sound_file->read(bufferdata.data() + total_bytesread,
                                                bytesrequested);
    total_bytesread += bytesread;

    if (m_loop) {
      if (m_sound_file->tell() >= m_sound_file->get_format().sample2bytes(m_loop->sample_end)) {
        std::cout << "loop\n";
        m_sound_file->seek_to_sample(m_loop->sample_beg);
      }
    } else {
      if (bytesread == 0) {
        /* EOF reached */
        break;
      }
    }
  } while(total_bytesread < STREAMFRAGMENTSIZE);

  if (total_bytesread > 0)
  {
    // upload data to the OpenAL buffer
    alBufferData(buffer, m_format, bufferdata.data(), static_cast<ALsizei>(total_bytesread), m_sound_file->get_format().get_rate());
    OpenALSystem::check_al_error("Couldn't refill audio buffer: ");

    // add buffer to the queue of this source
    alSourceQueueBuffers(m_source, 1, &buffer);
    OpenALSystem::check_al_error("Couldn't queue audio buffer: ");
  }
}

void
StreamSoundSource::update_queue()
{
  if (m_state != SourceState::Playing) { return; }

  if (!m_buffers_queued)
  {
    for(auto const& buffer : m_buffers) {
      fill_buffer_and_queue(buffer);
    }
    m_buffers_queued = true;
  }
  else
  { // fill the buffer queue with new data
    ALint processed = 0;
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);

    std::array<ALuint, STREAMFRAGMENTS> unqueue_buffers;
    alSourceUnqueueBuffers(m_source, processed, unqueue_buffers.data());
    OpenALSystem::warn_al_error("Couldn't unqueue audio buffer: ");

    for(int i = 0; i < processed; ++i) {
      fill_buffer_and_queue(unqueue_buffers[i]);

      // FIXME: actual processed sample count might be different if
      // buffers weren't filled completely
      m_total_samples_processed += (8 * static_cast<int>(STREAMFRAGMENTSIZE)
                                    / m_sound_file->get_format().get_channels()
                                    / m_sound_file->get_format().get_bits_per_sample());
    }
  }
}

void
StreamSoundSource::clear_queue()
{
  if (!m_buffers_queued) { return; }

  // stop the source to cause all buffers getting marked as
  // processed, so we can unqueue them
  OpenALSoundSource::finish();

  alSourcei(m_source, AL_BUFFER, AL_NONE);

  m_buffers_queued = false;
}

} // namespace wstsound

/* EOF */

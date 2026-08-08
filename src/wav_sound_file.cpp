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

#include <algorithm>
#include <bit>
#include <filesystem>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <sstream>

#include "sound_error.hpp"
#include "wav_sound_file.hpp"

namespace wstsound {

namespace {

template <class To, class From>
To bit_cast(const From& src) noexcept
{ // FIXME: replace with std::bit_cast when available
  To dst;
  memcpy(&dst, &src, sizeof(To));
  return dst;
}

template<typename T>
T read_le(std::istream& in)
{
  char data[sizeof(T)];

  if (!in.read(data, sizeof(data)))
  {
    std::ostringstream msg;
    msg << "Problem reading " << typeid(T).name() << ": " << strerror(errno);
    throw SoundError(msg.str());
  }
  else
  {
    if constexpr (std::endian::native == std::endian::big) {
      std::reverse(std::begin(data), std::end(data));
    }

    return bit_cast<T>(data);
  }
}

} // namespace

WavSoundFile::WavSoundFile(std::unique_ptr<std::istream> istream) :
  m_istream(std::move(istream)),
  m_datastart(),
  m_format(),
  m_size()
{
  char magic[4];
  if (!m_istream->read(magic, sizeof(magic)))
  {
    throw SoundError("Couldn't read file magic (not a wave file)");
  }

  if(strncmp(magic, "RIFF", 4) != 0)
  {
    printf("MAGIC: %4s.\n", magic);
    throw SoundError("file is not a RIFF wav file");
  }

  /*uint32_t wavelen =*/ read_le<uint32_t>(*m_istream);

  if (!m_istream->read( magic, sizeof(magic)))
  {
    throw SoundError("Couldn't read chunk header (not a wav file?)");
  }

  if(strncmp(magic, "WAVE", 4) != 0)
  {
    throw SoundError("file is not a valid RIFF/WAVE file");
  }

  char chunkmagic[4];
  uint32_t chunklen;

  // search audio data format chunk
  do {
    if (!m_istream->read(chunkmagic, sizeof(chunkmagic)))
    {
      throw SoundError("EOF while searching format chunk");
    }
    chunklen = read_le<uint32_t>(*m_istream);

    if (strncmp(chunkmagic, "fmt ", 4) == 0)
    {
      break;
    }
    else if (strncmp(chunkmagic, "fact", 4) == 0 ||
             strncmp(chunkmagic, "LIST", 4) == 0)
    {
      // skip chunk

      if (!m_istream->seekg(chunklen, std::ios::cur))
      {
        throw SoundError("EOF while searching fmt chunk");
      }
    }
    else
    {
      throw SoundError("complex WAVE files not supported");
    }
  } while(true);

  if (chunklen < 16)
    throw SoundError("Format chunk too short");

  // parse format
  uint16_t encoding = read_le<uint16_t>(*m_istream);
  if (encoding != 1)
  {
    std::ostringstream str;
    str << "WavSoundFile(): only PCM encoding supported, got " << encoding;
    throw SoundError(str.str());
  }

  uint16_t channels = read_le<uint16_t>(*m_istream);
  uint32_t rate = read_le<uint32_t>(*m_istream);
  /*uint32_t byterate =*/ read_le<uint32_t>(*m_istream);
  /*uint16_t blockalign =*/ read_le<uint16_t>(*m_istream);
  uint16_t bits_per_sample = read_le<uint16_t>(*m_istream);

  m_format = SoundFormat(rate, channels, bits_per_sample);

  if(chunklen > 16)
  {
    if(m_istream->seekg(chunklen-16, std::ios::cur).fail())
      throw SoundError("EOF while reading reast of format chunk");
  }

  // set file offset to DATA chunk data
  do {
    if (!m_istream->read(chunkmagic, sizeof(chunkmagic)))
      throw SoundError("EOF while searching data chunk");
    chunklen = read_le<uint32_t>(*m_istream);

    if(strncmp(chunkmagic, "data", 4) == 0)
      break;

    // skip chunk
    if(m_istream->seekg(chunklen, std::ios::cur).fail())
      throw SoundError("EOF while searching fmt chunk");
  } while(true);

  m_datastart = m_istream->tellg();
  m_size = static_cast<size_t>(chunklen);
}

WavSoundFile::~WavSoundFile()
{
}

void
WavSoundFile::seek_to_sample(int sample)
{
  std::streamoff byte_pos = m_format.sample2bytes(sample);

  if (!m_istream->seekg(m_datastart + byte_pos, std::ios::beg)) {
    throw SoundError("Couldn't seek to data start");
  }
}

size_t
WavSoundFile::read(void* buffer, size_t buffer_size)
{
  if (!m_istream->read(static_cast<char*>(buffer), buffer_size))
  {
    if (!m_istream->eof()) {
      throw SoundError("read error while reading samples");
    } else {
      // tellg() will return -1 unless eofbit is cleared
      m_istream->clear();
    }
    return m_istream->gcount();
  }
  else
  {
    std::streamsize bytesread = m_istream->gcount();

#ifdef __clang__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunreachable-code"
#endif
    // handle endian swaping
    if constexpr (std::endian::native == std::endian::big) {
      for(std::streamsize i = 0; i < bytesread; i += sizeof(uint16_t)) {
        std::reverse(static_cast<char*>(buffer),
                     static_cast<char*>(buffer) + sizeof(uint16_t));
      }
    }
#ifdef __clang__
#  pragma GCC diagnostic pop
#endif

    return bytesread;
  }
}

size_t
WavSoundFile::tell() const
{
  return static_cast<size_t>(m_istream->tellg() - m_datastart);
}

} // namespace wstsound

/* EOF */

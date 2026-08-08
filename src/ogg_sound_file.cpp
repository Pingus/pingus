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

#include "ogg_sound_file.hpp"

#include <algorithm>
#include <filesystem>
#include <sstream>

#include "sound_error.hpp"

namespace wstsound {

OggSoundFile::OggSoundFile(std::unique_ptr<std::istream> istream) :
  m_istream(std::move(istream)),
  m_file_size(),
  m_vorbis_file(),
  m_format(),
  m_size()
{
  // get the file size
  m_istream->seekg(0, std::ios::end);
  m_file_size = static_cast<size_t>(m_istream->tellg());
  m_istream->seekg(0, std::ios::beg);

  ov_callbacks callbacks = { cb_read, cb_seek, cb_close, cb_tell };

  int ret = ov_open_callbacks(this, &m_vorbis_file, nullptr, 0, callbacks);

  if (ret != 0)
  {
    std::ostringstream str;

    switch(ret)
    {
      case OV_EREAD:
        str << "OV_EREAD error while opening ogg";
        break;

      case OV_ENOTVORBIS:
        str << "OV_ENOTVORBIS error while opening ogg";
        break;

      case OV_EVERSION:
        str << "OV_EVERSION error while opening ogg";
        break;

      case OV_EBADHEADER:
        str << "OV_EBADHEADER error while opening ogg";
        break;

      case OV_EFAULT:
        str << "OV_EFAULT error while opening ogg";
        break;

      default:
        str << "Unknown error while opening ogg: " << ret;
        break;
    }

    throw SoundError(str.str());
  }

  vorbis_info* vi = ov_info(&m_vorbis_file, -1);

  m_format = SoundFormat(static_cast<int>(vi->rate), vi->channels, 16);
  m_size = m_format.sample2bytes(static_cast<int>(ov_pcm_total(&m_vorbis_file, -1)));
}

OggSoundFile::~OggSoundFile()
{
  ov_clear(&m_vorbis_file);
}

size_t
OggSoundFile::read(void* _buffer, size_t buffer_size)
{
  char* buffer = reinterpret_cast<char*> (_buffer);
  int section = 0;
  size_t totalBytesRead= 0;

  while(buffer_size>0)
  {
    long bytesRead
      = ov_read(&m_vorbis_file, buffer, static_cast<int>(buffer_size),
                std::endian::native == std::endian::big, 2, 1,
                &section);

    if (bytesRead == 0)
    {
      break;
    }

    buffer_size -= bytesRead;
    buffer += bytesRead;
    totalBytesRead += bytesRead;
  }

  return totalBytesRead;
}

size_t
OggSoundFile::tell() const
{
  return m_format.sample2bytes(static_cast<int>(ov_pcm_tell(const_cast<OggVorbis_File*>(&m_vorbis_file))));
}

void
OggSoundFile::seek_to_sample(int sample)
{
  ov_pcm_seek_lap(&m_vorbis_file, sample);
}

size_t
OggSoundFile::cb_read(void* ptr, size_t size, size_t nmemb, void* userdata)
{
  OggSoundFile& ogg = *reinterpret_cast<OggSoundFile*>(userdata);

  if (!ogg.m_istream->read(static_cast<char*>(ptr), size * nmemb))
  {
    if (!ogg.m_istream->eof()) {
      return -1;
    } else {
      // tellg() will return -1 unless eofbit is cleared
      ogg.m_istream->clear();
    }
  }

  return static_cast<int>(ogg.m_istream->gcount());
}

int
OggSoundFile::cb_seek(void* userdata, ogg_int64_t offset, int whence)
{
  OggSoundFile& ogg = *reinterpret_cast<OggSoundFile*>(userdata);

  switch(whence)
  {
    case SEEK_SET:
      if (!ogg.m_istream->seekg(offset, std::ios::beg))
        return -1;
      break;

    case SEEK_CUR:
      if (!ogg.m_istream->seekg(offset, std::ios::cur))
        return -1;
      break;

    case SEEK_END:
      if (!ogg.m_istream->seekg(offset, std::ios::end))
        return -1;
      break;

    default:
      return -1;
  }
  return 0;
}

int
OggSoundFile::cb_close(void* userdata)
{
  OggSoundFile& ogg = *reinterpret_cast<OggSoundFile*>(userdata);

  ogg.m_istream.reset();

  return 0;
}

long
OggSoundFile::cb_tell(void* userdata)
{
  OggSoundFile& ogg = *reinterpret_cast<OggSoundFile*>(userdata);
  return static_cast<long>(ogg.m_istream->tellg());
}

} // namespace wstsound

/* EOF */

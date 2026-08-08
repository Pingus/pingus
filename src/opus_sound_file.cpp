/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2018 Ingo Ruhnke <grumbel@gmail.com>
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

#include "opus_sound_file.hpp"

#include <iostream>
#include <sstream>

#include "sound_error.hpp"

namespace wstsound {

OpusSoundFile::OpusSoundFile(std::unique_ptr<std::istream> istream) :
  m_istream(std::move(istream)),
  m_file_size(0),
  m_opus_file(nullptr),
  m_format(),
  m_size(0)
{
  // get the file size
  m_istream->seekg(0, std::ios::end);
  m_file_size = static_cast<size_t>(m_istream->tellg());
  m_istream->seekg(0, std::ios::beg);

  OpusFileCallbacks callbacks = { cb_read, cb_seek, cb_tell, cb_close };

  int err;
  m_opus_file = op_open_callbacks(this, &callbacks, nullptr, 0, &err);
  if (m_opus_file == nullptr)
  {
    std::ostringstream str;

    switch(err)
    {
      case OP_EREAD:
      case OP_EFAULT:
      case OP_EIMPL:
      case OP_EINVAL:
      case OP_ENOTFORMAT:
      case OP_EBADHEADER:
      case OP_EVERSION:
      case OP_EBADLINK:
      case OP_EBADTIMESTAMP:
      default:
        str << "op_open_callbacks() failed with " << err;
    }

    throw SoundError(str.str());
  }

  m_format = SoundFormat(48000, op_channel_count(m_opus_file, -1), 16);

  ogg_int64_t pcm_total = op_pcm_total(m_opus_file, -1);
  if (pcm_total < 0) {
    op_free(m_opus_file);
    throw SoundError("OpusSoundFile: op_pcm_total() failure");
  }
  m_size = m_format.sample2bytes(static_cast<int>(pcm_total));
}

OpusSoundFile::~OpusSoundFile()
{
  op_free(m_opus_file);
}

size_t
OpusSoundFile::read(void* buffer, size_t buffer_size)
{
  // returns number of samples per channel, not bytes
  int const pcm_read = op_read(m_opus_file,
                               static_cast<opus_int16*>(buffer),
                               static_cast<int>(buffer_size / sizeof(opus_int16)),
                               nullptr);
  if (pcm_read < 0) {
    std::cerr << "OpusSoundFile: decode failure: " << pcm_read << std::endl;
    return 0;
  }

  return m_format.sample2bytes(pcm_read);
}

size_t
OpusSoundFile::tell() const
{
  return m_format.sample2bytes(static_cast<int>(op_pcm_tell(m_opus_file)));
}

void
OpusSoundFile::seek_to_sample(int sample)
{
  op_pcm_seek(m_opus_file, sample);
}

int
OpusSoundFile::cb_read(void* stream, unsigned char* buffer, int nbytes)
{
  OpusSoundFile& opus = *reinterpret_cast<OpusSoundFile*>(stream);

  if (!opus.m_istream->read(reinterpret_cast<char*>(buffer), nbytes))
  {
    if (!opus.m_istream->eof()) {
      return -1;
    } else {
      // tellg() will return -1 unless eofbit is cleared
      opus.m_istream->clear();
    }
  }

  return static_cast<int>(opus.m_istream->gcount());
}

int
OpusSoundFile::cb_seek(void* stream, opus_int64 offset, int whence)
{
  OpusSoundFile& opus = *reinterpret_cast<OpusSoundFile*>(stream);

  switch(whence)
  {
    case SEEK_SET:
      if (!opus.m_istream->seekg(offset, std::ios::beg)) {
        return -1;
      }
      break;

    case SEEK_CUR:
      if (!opus.m_istream->seekg(offset, std::ios::cur)) {
        return -1;
      }
      break;

    case SEEK_END:
      if (!opus.m_istream->seekg(offset, std::ios::end)) {
        return -1;
      }
      break;

    default:
      std::cerr << "OpusSoundFile: seek failure: <unknown>\n";
      return -1;
  }
  return 0;
}

int
OpusSoundFile::cb_close(void* stream)
{
  OpusSoundFile& opus = *reinterpret_cast<OpusSoundFile*>(stream);
  opus.m_istream.reset();
  return 0;
}

opus_int64
OpusSoundFile::cb_tell(void* stream)
{
  OpusSoundFile& opus = *reinterpret_cast<OpusSoundFile*>(stream);
  return static_cast<opus_int64>(opus.m_istream->tellg());
}

} // namespace wstsound

/* EOF */

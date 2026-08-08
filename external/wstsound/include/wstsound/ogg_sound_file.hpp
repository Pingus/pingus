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

#ifndef HEADER_WINDSTILLE_SOUND_OGG_SOUND_FILE_HPP
#define HEADER_WINDSTILLE_SOUND_OGG_SOUND_FILE_HPP

#include <vorbis/vorbisfile.h>
#include <istream>

#include "sound_file.hpp"

namespace wstsound {

class OggSoundFile : public SoundFile
{
public:
  OggSoundFile(std::unique_ptr<std::istream> istream);
  ~OggSoundFile() override;

  size_t read(void* buffer, size_t buffer_size) override;
  size_t tell() const override;
  void seek_to_sample(int sample) override;
  size_t get_size() const override { return m_size; }
  SoundFormat get_format() const override { return m_format; }

private:
  // I/O functions for ogg
  static size_t cb_read(void* ptr, size_t size, size_t nmemb, void* source);
  static int    cb_seek(void* source, ogg_int64_t offset, int whence);
  static int    cb_close(void* source);
  static long   cb_tell(void* source);

  std::unique_ptr<std::istream> m_istream;
  size_t m_file_size;
  OggVorbis_File m_vorbis_file;
  SoundFormat m_format;
  size_t m_size; /// size in bytes

private:
  OggSoundFile(const OggSoundFile&);
  OggSoundFile& operator=(const OggSoundFile&);
};

} // namespace wstsound

#endif

/* EOF */

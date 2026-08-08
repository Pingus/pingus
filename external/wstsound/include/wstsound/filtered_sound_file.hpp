/*
**  Windstille - A Sci-Fi Action-Adventure Game
**  Copyright (C) 2010 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_WINDSTILLE_SOUND_FILTERED_SOUND_FILE_HPP
#define HEADER_WINDSTILLE_SOUND_FILTERED_SOUND_FILE_HPP

#include <memory>
#include <string>

#include "sound_file.hpp"

namespace wstsound {

class FilteredSoundFile : public SoundFile
{
public:
  FilteredSoundFile(std::unique_ptr<SoundFile> sound_file);
  ~FilteredSoundFile() override;

  size_t read(void* buffer, size_t buffer_size) override;
  void seek_to_sample(int sample) override;
  size_t get_size() const override;
  SoundFormat get_format() const override;

private:
  std::unique_ptr<SoundFile> m_sound_file;

private:
  FilteredSoundFile(const FilteredSoundFile&);
  FilteredSoundFile& operator=(const FilteredSoundFile&);
};

} // namespace wstsound

#endif

/* EOF */

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

/** Used SDL_mixer and glest source as reference */

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string.h>

#include "modplug_sound_file.hpp"
#include "mp3_sound_file.hpp"
#include "ogg_sound_file.hpp"
#include "opus_sound_file.hpp"
#include "sound_error.hpp"
#include "wav_sound_file.hpp"

namespace wstsound {

std::unique_ptr<SoundFile>
SoundFile::from_stream(std::unique_ptr<std::istream> istream)
{
  uint8_t magic[64];

  if (!istream->read(reinterpret_cast<char*>(magic), sizeof(magic))) {
    throw SoundError("Couldn't read magic, file too short");
  } else {
    // reset the stream before handing it over
    istream->seekg(0, std::ios::beg);

    // FIXME: this is a bit of a hack, there are probably better ways
    // to tell OggVorbis and Opus appart
    if (strncmp(reinterpret_cast<char*>(magic), "RIFF", 4) == 0) {
      return std::make_unique<WavSoundFile>(std::move(istream));
    } else if ((magic[0] == 0xff && (magic[1] == 0xfb || magic[1] == 0xf3 || magic[1] == 0xf2)) ||
               (magic[0] == 0x49 && magic[1] == 0x44 && magic[2] == 0x33)) {
      return std::make_unique<MP3SoundFile>(std::move(istream));
    } else if (strncmp(reinterpret_cast<char*>(magic), "OggS", 4) == 0 &&
               strncmp(reinterpret_cast<char*>(magic) + 28, "OpusHead", 8) == 0) {
      return std::make_unique<OpusSoundFile>(std::move(istream));
    } else if (strncmp(reinterpret_cast<char*>(magic), "OggS", 4) == 0 &&
               strncmp(reinterpret_cast<char*>(magic) + 29, "vorbis", 4) == 0) {
      return std::make_unique<OggSoundFile>(std::move(istream));
    } else if (strncmp(reinterpret_cast<char*>(magic), "IMPM", 4) == 0) {
      return std::make_unique<ModplugSoundFile>(std::move(istream));
    } else {
      throw SoundError("Unknown file format");
    }
  }
}

std::unique_ptr<SoundFile>
SoundFile::from_file(std::filesystem::path const& filename)
{
  std::ifstream in(filename, std::ios::binary);

  if (!in) {
    std::ostringstream msg;
    msg << "Couldn't open '" << filename << "'";
    throw SoundError(msg.str());
  } else {
    try {
      return from_stream(std::make_unique<std::ifstream>(std::move(in)));
    } catch(std::exception& e) {
      std::ostringstream msg;
      msg << "Couldn't read '" << filename << "': " << e.what();
      throw SoundError(msg.str());
    }
  }
}

float
SoundFile::get_duration() const
{
  return static_cast<float>(get_sample_duration()) / static_cast<float>(get_format().get_rate());
}

int
SoundFile::get_sample_duration() const
{
  return 8 * static_cast<int>(get_size()) / get_format().get_channels() / get_format().get_bits_per_sample();
}

} // namespace wstsound

/* EOF */

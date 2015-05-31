// Pingus - A free Lemmings clone
// Copyright (C) 2002 Ingo Ruhnke <grumbel@gmail.com>
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

#include "engine/sound/sound_res_mgr.hpp"

#include "pingus/globals.hpp"
#include "pingus/path_manager.hpp"
#include "util/log.hpp"

SoundResMgr::SoundMap SoundResMgr::sound_map;

SoundHandle
SoundResMgr::load(const std::string& name)
{
  SoundMap::iterator i = sound_map.find(name);

  if (i == sound_map.end())
  {
    std::string filename = g_path_manager.complete("sounds/" + name + ".wav");
    Mix_Chunk* chunk = Mix_LoadWAV(filename.c_str());
    log_info("SoundResMgr: Loading sound from disk: %1% -> %2%", name, filename);
    if (!chunk)
      log_info("Error: %1%", Mix_GetError());

    sound_map[name] = chunk;
    return chunk;
  }
  else
  {
    log_info("SoundResMgr: Loading sound from cache: %1%", name);
    return i->second;
  }

  return 0;
}

void SoundResMgr::free_sound_map()
{
  for (SoundMap::iterator i = sound_map.begin(); i != sound_map.end(); ++i)
  {
    Mix_FreeChunk(i->second);
  }
}

/* EOF */

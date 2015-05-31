// Pingus - A free Lemmings clone
// Copyright (C) 1998-2011 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_SYSTEM_SDL_HPP
#define HEADER_PINGUS_ENGINE_SYSTEM_SDL_HPP

#include <SDL.h>

#include "pingus/options.hpp"

class Size;

class SDLSystem
{
private:
public:
  SDLSystem();
  ~SDLSystem();

  void create_window(FramebufferType framebuffer_type, const Size& size, bool fullscreen, bool resizable);

private:
  SDLSystem(const SDLSystem&);
  SDLSystem& operator=(const SDLSystem&);
};

#endif

/* EOF */

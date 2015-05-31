// Pingus - A free Lemmings clone
// Copyright (C) 2008 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_SDL_FRAMEBUFFER_SURFACE_IMPL_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_SDL_FRAMEBUFFER_SURFACE_IMPL_HPP

#include "engine/display/framebuffer_surface.hpp"

class SDLFramebufferSurfaceImpl : public FramebufferSurfaceImpl
{
private:
  SDL_Texture* m_texture;
  int m_width;
  int m_height;

public:
  SDLFramebufferSurfaceImpl(SDL_Renderer* renderer, SDL_Surface* src);
  ~SDLFramebufferSurfaceImpl();

  int get_width()  const { return m_width;; }
  int get_height() const { return m_height; }

  SDL_Texture* get_texture() const { return m_texture; }

private:
  SDLFramebufferSurfaceImpl(const SDLFramebufferSurfaceImpl&);
  SDLFramebufferSurfaceImpl & operator=(const SDLFramebufferSurfaceImpl&);
};

#endif

/* EOF */

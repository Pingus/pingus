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

#include "engine/display/sdl_framebuffer_surface_impl.hpp"

SDLFramebufferSurfaceImpl::SDLFramebufferSurfaceImpl(SDL_Renderer* renderer, SDL_Surface* src) :
  m_texture(SDL_CreateTextureFromSurface(renderer, src)),
  m_width(src->w),
  m_height(src->h)
{
}

SDLFramebufferSurfaceImpl::~SDLFramebufferSurfaceImpl()
{
  SDL_DestroyTexture(m_texture);
}

/* EOF */

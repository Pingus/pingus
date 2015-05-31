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

#ifndef HEADER_PINGUS_ENGINE_DISPLAY_FRAMEBUFFER_SURFACE_HPP
#define HEADER_PINGUS_ENGINE_DISPLAY_FRAMEBUFFER_SURFACE_HPP

#include "engine/display/surface.hpp"

class FramebufferSurfaceImpl
{
public:
  FramebufferSurfaceImpl() {}
  virtual ~FramebufferSurfaceImpl() {}

  virtual int get_width()  const =0;
  virtual int get_height() const =0;
};

class FramebufferSurface
{
public:
  FramebufferSurface();
  FramebufferSurface(FramebufferSurfaceImpl* impl);
  ~FramebufferSurface();

  int  get_width()  const;
  int  get_height() const;
  Size get_size()   const;

  FramebufferSurfaceImpl* get_impl() const;

  bool operator==(const FramebufferSurface& other) const;

  operator bool() const;

  long use_count() const;

private:
  std::shared_ptr<FramebufferSurfaceImpl> impl;
};

#endif

/* EOF */

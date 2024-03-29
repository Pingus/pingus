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

#include "engine/display/framebuffer_surface.hpp"

namespace pingus {

FramebufferSurface::FramebufferSurface() :
  impl()
{
}

FramebufferSurface::FramebufferSurface(FramebufferSurfaceImpl* impl_) :
  impl(impl_)
{
}

FramebufferSurface::~FramebufferSurface()
{
}

int
FramebufferSurface::get_width()  const
{
  if (impl.get())
    return impl->get_width();
  else
    return 0;
}

int
FramebufferSurface::get_height() const
{
  if (impl.get())
    return impl->get_height();
  else
    return 0;
}

geom::isize
FramebufferSurface::get_size() const
{
  if (impl.get())
    return geom::isize(impl->get_width(), impl->get_height());
  else
    return geom::isize(0, 0);
}

FramebufferSurfaceImpl*
FramebufferSurface::get_impl() const
{
  return impl.get();
}

bool
FramebufferSurface::operator==(FramebufferSurface const& other) const
{
  return impl == other.impl;
}

FramebufferSurface::operator bool() const
{
  return impl != nullptr;
}

long
FramebufferSurface::use_count() const
{
  return impl.use_count();
}

} // namespace pingus

/* EOF */

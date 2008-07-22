//  Pingus - A free Lemmings clone
//  Copyright (C) 2008 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef HEADER_FRAMEBUFFER_SURFACE_HPP
#define HEADER_FRAMEBUFFER_SURFACE_HPP

#include <boost/smart_ptr.hpp>
#include "surface.hpp"
#include "../math/size.hpp"

class FramebufferSurfaceImpl
{
public:
  FramebufferSurfaceImpl() {}
  virtual ~FramebufferSurfaceImpl() {}

  virtual int get_width()  const =0;
  virtual int get_height() const =0;
  virtual Surface to_surface() const =0; 
};

/** */
class FramebufferSurface
{
public:
  FramebufferSurface() {}
  FramebufferSurface(FramebufferSurfaceImpl* impl) : impl(impl) {}
  ~FramebufferSurface() {}

  int  get_width()  const { return impl->get_width();  }
  int  get_height() const { return impl->get_height(); }
  Size get_size()   const { return Size(impl->get_width(), impl->get_height()); }

  FramebufferSurfaceImpl* get_impl() const { return impl.get(); }

  bool operator==(const FramebufferSurface& other) const { return impl == other.impl; }

  operator bool() const { return impl.get() != 0; }

  long use_count() const { return impl.use_count(); }

  Surface to_surface() const { return impl->to_surface(); }
private:
  boost::shared_ptr<FramebufferSurfaceImpl> impl;
};

#endif

/* EOF */

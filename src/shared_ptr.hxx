//  $Id$
// 
//  Flexlay - A Generic 2D Game Editor
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
// 
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_SHARED_PTR_HXX
#define HEADER_SHARED_PTR_HXX

#include <assert.h>

/** */
template<typename T>
class SharedPtr
{
private:
  T* ptr;
public:
  template<typename Parent> friend class SharedPtr;

  SharedPtr() : ptr(0) {}
  SharedPtr(T* p) : ptr(p) {}
  
  template <typename Parent>
  SharedPtr(const SharedPtr<Parent>& p) : ptr(p.ptr) {}

  T& operator*() { assert(ptr); return *ptr; }
  T const& operator*() const { assert(ptr); return *ptr; }

  T* operator->() { assert(ptr); return ptr; }
  T const* operator->() const { assert(ptr); return ptr; }

  T* get() const { return ptr; }
};

#endif

/* EOF */

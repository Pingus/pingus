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

// This code is a modified version of CL_SharedPtr from ClanLib

class UnknownSharedPtrImpl
{
public:
  UnknownSharedPtrImpl() : count(1) {}
  virtual ~UnknownSharedPtrImpl() {}

  unsigned count;
};

template<typename U>
class SharedPtrImpl : public UnknownSharedPtrImpl
{
public:
  SharedPtrImpl() : ptr(0) {}

  U* ptr;
};

template<typename T, typename U>
class SharedPtrDelete : public SharedPtrImpl<T>
{
public:
  SharedPtrDelete(U* ptr) { this->ptr = ptr; }
  ~SharedPtrDelete() { delete this->ptr; }
};

class UnknownSharedPtr;

/** */
template<typename T, typename U = T>
class SharedPtr
{
public:
  SharedPtr() : impl(0) {}
  SharedPtr(const SharedPtr<T, U>& ptr) : impl(ptr.impl) { increment(); }
  SharedPtr(const UnknownSharedPtr& ptr) : impl(0) {
    if (ptr.impl) {
      impl = dynamic_cast<SharedPtrImpl<T>*>(ptr.impl);
      increment();
    }
  }
  
  template<typename D>
  explicit SharedPtr(D* ptr) : impl(0) {
    if (ptr)
      impl = new SharedPtrDelete<T, D>(ptr);
  }
  explicit SharedPtr(SharedPtrImpl<T>* impl) : impl(impl) { increment(); }
  
  ~SharedPtr() { decrement(); }

  bool operator==(const T* ptr) const { return ptr == (impl ? impl.ptr : 0); }
  bool operator==(const SharedPtr<T, U>& ptr) const { return ptr.impl == impl; }

  SharedPtr<T>& operator=(const SharedPtr<T>& ptr) {
    if (ptr.impl != impl) {
      decrement();
      impl = ptr.impl;
      increment();
    }
    return *this;
  }
  SharedPtr<T>& operator=(const UnknownSharedPtr& ptr) {
    if (ptr.impl != impl) {
      decrement();
      if (ptr.impl) {
	impl = dynamic_cast<SharedPtrImpl<T>*>(ptr.impl);
	increment();
      }
    }
    return *this;
  }

  U& operator*() { assert(impl); return *((U*)impl->ptr); }
  U const& operator*() const { assert(impl); return *((const U*)impl->ptr); }

  U* operator->() { assert(impl); return (U*)(impl->ptr); }
  U const* operator->() const { assert(impl); return (const U*)(impl->ptr); }

  const U* get() const { return (U*)(impl ? impl->ptr : 0); }

  operator bool() const { return impl ? impl->ptr : 0; }

private:
  SharedPtrImpl<T>* impl;

  void increment() {
    if (impl)
      impl->count++;
  }
  void decrement() {
    if (impl && --impl->count == 0) {
      delete impl;
      impl = 0;
    }
  }

  friend class UnknownSharedPtr;
};

class UnknownSharedPtr
{
public:
  UnknownSharedPtr() : impl(0) {}
  template<typename T>
  explicit UnknownSharedPtr(const SharedPtr<T>& ptr) : impl(ptr.impl) {
    increment();
  }
  ~UnknownSharedPtr() { decrement(); }

  bool operator==(const UnknownSharedPtr& ptr) const { return ptr.impl == impl; }
  template <typename T, typename U>
  bool operator==(const SharedPtr<T, U>& ptr) const { return ptr.impl == impl; }

  template<typename T, typename U>
  UnknownSharedPtr& operator=(const SharedPtr<T, U>& ptr) {
    if (ptr.impl != impl) {
      decrement();
      impl = ptr.impl;
      increment();
    }
    return *this;
  }

private:
  UnknownSharedPtrImpl* impl;

  void increment() {
    if (impl)
      impl->count++;
  }
  void decrement() {
    if (impl && --impl->count == 0) {
      delete impl;
      impl = 0;
    }
  }
};

#endif

/* EOF */

// Pingus - A free Lemmings clone
// Copyright (C) 1999 Ingo Ruhnke <grumbel@gmail.com>
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

#ifndef HEADER_PINGUS_UTIL_MEMORY_POOL_HPP
#define HEADER_PINGUS_UTIL_MEMORY_POOL_HPP

#include <assert.h>
#include <stddef.h>
#include <vector>

/** MemoryPool allows the allocation of small objects on a previous
    allocated chunk of memeroy, thus reducing the amount of new/delete
    calls that have do be done and providing a speed up. */
template<class T>
class MemoryPool
{
private:
  typedef std::vector<T*> Objects;
  Objects objects;

  typedef std::vector<char*> Chunks;
  Chunks chunks;

  size_t chunk_size;
  size_t next_free;

  char* allocate(size_t size)
  {
    assert(size <= chunk_size);

    if (chunks.empty() ||
        (next_free + size) > chunk_size)
    {
      char* chunk = new char[chunk_size];
      chunks.push_back(chunk);
      next_free = 0;
    }

    char* ptr = chunks.back() + next_free;
    next_free += size;
    return ptr;
  }

  T* keep(T* t)
  {
    objects.push_back(t);
    return t;
  }

public:
  MemoryPool(size_t chunk_size_ = 16384) :
    objects(),
    chunks(),
    chunk_size(chunk_size_),
    next_free(0)
  {}

  ~MemoryPool()
  {
    clear();
  }

  void clear()
  {
    for(typename Objects::reverse_iterator i = objects.rbegin(); i != objects.rend(); ++i)
      (*i)->~T();
    objects.clear();

    // FIXME: We don't have to delete the chunks, instead we should
    // just reset the pointer to start and reuse them
    for(typename Chunks::reverse_iterator i = chunks.rbegin(); i != chunks.rend(); ++i)
    {
      delete[] *i;
    }
    chunks.clear();

    next_free = 0;
  }

  template<class C>
  T* create() { return keep(new (allocate(sizeof(C))) C()); }
  template<class C, class Arg1>
  T* create(const Arg1& arg1) { return keep(new (allocate(sizeof(C))) C(arg1)); }
  template<class C, class Arg1, class Arg2>
  T* create(const Arg1& arg1, const Arg2& arg2) { return keep(new (allocate(sizeof(C))) C(arg1, arg2)); }
  template<class C, class Arg1, class Arg2, class Arg3>
  T* create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3) { return keep(new (allocate(sizeof(C))) C(arg1, arg2, arg3)); }
  template<class C, class Arg1, class Arg2, class Arg3, class Arg4>
  T* create(const Arg1& arg1, const Arg2& arg2, const Arg3& arg3, const Arg4& arg4) { return keep(new (allocate(sizeof(C))) C(arg1, arg2, arg3, arg4)); }

private:
  MemoryPool (const MemoryPool&);
  MemoryPool& operator= (const MemoryPool&);
};

#endif

/* EOF */

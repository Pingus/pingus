// Pingus - A free Lemmings clone
// Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
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

#include <iostream>

#include "util/memory_pool.hpp"

struct Thing {
  int i;

  Thing(int i_) : i(i_) {
    std::cout << "Constructing thing: " << i << std::endl;
  }

  ~Thing() {
    std::cout << "Destructing thing: " << i << std::endl;
  }
};

int main()
{
  MemoryPool<Thing> pool;
  pool.create<Thing>(5);
  pool.create<Thing>(3);
  pool.create<Thing>(2);

  return 0;
}

/* EOF */

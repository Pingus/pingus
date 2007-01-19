/*  $Id$
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#include <iostream>
#include "resource.hpp"
#include "collision_mask.hpp"

CollisionMask::CollisionMask()
{

}

CollisionMask::CollisionMask(const std::string& name)
{
  //std::cout << "CollisionMask: " << name << std::endl;
  pixelbuffer = Resource::load_pixelbuffer(name);
}

CollisionMask::~CollisionMask()
{
}  

int
CollisionMask::get_width() const
{
  return pixelbuffer.get_width();
}

int
CollisionMask::get_height() const
{
  return pixelbuffer.get_height();
}

PixelBuffer
CollisionMask::get_pixelbuffer() const
{
  return pixelbuffer;
}

/* EOF */

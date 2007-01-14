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

#include "vector2i.hpp"

Vector2i
Vector2i::operator+ (const Vector2i& add) const
{
  return Vector2i(x + add.x, y + add.y);
}

Vector2i
Vector2i::operator- (const Vector2i& sub) const
{
  return Vector2i(x - sub.x, y - sub.y);
}

Vector2i
Vector2i::operator* (int  mul) const
{
  return Vector2i(x * mul, y * mul);
}

Vector2i&
Vector2i::operator+= (const Vector2i& add)
{
  x += add.x;
  y += add.y;
  return *this;
}

Vector2i&
Vector2i::operator-= (const Vector2i& sub)
{
  x -= sub.x;
  y -= sub.y;
  return *this;
}

Vector2i&
Vector2i::operator*= (int mul)
{
  x *= mul;
  y *= mul;
  return *this;
}

/* EOF */

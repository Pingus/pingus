//  $Id: AnimCounter.hh,v 1.5 2002/06/07 09:42:16 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef ANIM_COUNTER_HH
#define ANIM_COUNTER_HH

#include "Counter.hh"

// AnimCounter is an inherit class from Counter, it act like a normal
// counter, but it only increase its counter if a specific amount of
// time is passed. The AnimCounter is mostly used in the sprite
/// animation, to keep them at a constant speed.
class AnimCounter : public Counter
{
private:
  ///
  bool enough_time_passed(void) const;
  ///
  int last_time;
  ///
  unsigned int speed;
public:
  ///
  AnimCounter(int s);
  ///
  AnimCounter();
  ///
  int operator=(int i);
  ///
  int operator++();

  /// Set the number of miliseconds between a count increase
  void set_speed(int s);

  /// Returs the speed which is used for this counter
  int  get_speed() const;
}///
;

#endif

/* EOF */

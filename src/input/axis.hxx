//  $Id: axis.hxx,v 1.10 2003/04/19 10:23:18 torangan Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2000 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_INPUT_AXIS_HXX
#define HEADER_PINGUS_INPUT_AXIS_HXX

#include "../pingus.hxx"

namespace Input {

  /// abstract base class which defines the axis interface
  class Axis {

    public:
      Axis () { }
      virtual ~Axis () { }

      /// yields the position of the axis in [-1;1]
      virtual const float& get_pos () const =0;

      /// yields the angle of axis in [0;360[ degree
      virtual const float& get_angle () const =0;

      virtual void  update(float) =0;

    private:
      Axis (const Axis&);
      Axis& operator= (const Axis&);
  };

}

#endif

/* EOF */

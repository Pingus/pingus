//  $Id: inverted_axis.hxx,v 1.5 2003/10/20 19:28:55 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_INVERTED_AXIS_HXX
#define HEADER_PINGUS_INPUT_INVERTED_AXIS_HXX

#include "../axis.hxx"

namespace Pingus {
namespace Input {
namespace Axes {

/**
   @brief decorator class inverting the angle of an axis

   XML definition: <inverted-axis> <axis> </inverted-axis>
*/
class InvertedAxis : public Axis {

private:
  Axis* const axis;
  float       angle;

public:
  InvertedAxis (Axis* axis_);
  ~InvertedAxis ();

  virtual const float& get_pos   () const;
  virtual const float& get_angle () const;

  virtual void  update (float delta);

private:
  InvertedAxis (const InvertedAxis&);
  InvertedAxis& operator= (const InvertedAxis&);
};

} // namespace Axes
} // namespace Input
} // namespace Pingus

#endif

/* EOF */

//  $Id: inverted_scroller.hxx,v 1.5 2003/10/20 19:28:55 grumbel Exp $
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

#ifndef HEADER_PINGUS_INPUT_INVERTED_SCROLLER_HXX
#define HEADER_PINGUS_INPUT_INVERTED_SCROLLER_HXX

#include "../scroller.hxx"

namespace Pingus {
namespace Input {
namespace Scrollers {

/**
   @brief inverts the results of the contained scroller

   XML definition: <inverted-scroller invert-x="0/1" invert-y="0/1" speed="?"> <scroller> </inverted-scroller>

   Wheter the X and/or the Y axis shall be inverted must be specified explizitly.
*/
class InvertedScroller : public Scroller {
private:
  Scroller* const scroller;

  const bool invert_x;
  const bool invert_y;

  float x_pos;
  float y_pos;

public:
  InvertedScroller (Scroller* scroller_, bool invert_x_, bool invert_y_);
  ~InvertedScroller ();

  const float& get_x_delta () const;
  const float& get_y_delta () const;

  void  get_delta (float& x, float& y) const;

  void  update (float delta);

private:
  InvertedScroller (const InvertedScroller&);
  InvertedScroller& operator= (const InvertedScroller&);
};

} // namespace Scroller
} // namespace Input
} // namespace Pingus

#endif

/* EOF */

//  $Id: dummy_scroller.hxx,v 1.3 2002/08/23 15:49:55 torangan Exp $
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

#ifndef HEADER_PINGUS_INPUT_DUMMY_SCROLLER_HXX
#define HEADER_PINGUS_INPUT_DUMMY_SCROLLER_HXX

#include "scroller.hxx"

namespace Input {

  /**
    @brief dummy class to be used if an Scroller is required but none defined
    
    XML definition: none
   */
  class DummyScroller : public Scroller {
    private:
      const float delta;
      
    public:
      
      DummyScroller () : delta(0) { }
      
      const float& get_x_delta () const { return delta; }
      const float& get_y_delta () const { return delta; }
      
      void  get_delta (float& x_delta, float& y_delta) const { x_delta = delta; y_delta = delta; }
      
      void  update (float) { }
      
    private:
      DummyScroller (const DummyScroller&);
      DummyScroller operator= (const DummyScroller&);
  };
}

#endif

/* EOF */

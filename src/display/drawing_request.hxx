//  $Id$
// 
//  Pingus - A free Lemmings clone
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

#ifndef HEADER_DRAWING_REQUEST_HXX
#define HEADER_DRAWING_REQUEST_HXX

#include <ClanLib/Core/Math/cl_vector.h>

class CL_GraphicContext;

/** 
 */
class DrawingRequest
{
protected:
  CL_Vector pos;
public:
  DrawingRequest(const CL_Vector& pos_) : pos(pos_) {}
  
  virtual void draw(CL_GraphicContext* gc) = 0;
  
  /** Returns true if the request contains an alpha channel and needs
      to be drawn in order */
  virtual bool has_alpha() { return true; }

  /** Returns the position at which the request should be drawn */
  virtual float get_z_pos() { return pos.z; }
private:
  DrawingRequest (const DrawingRequest&);
  DrawingRequest& operator= (const DrawingRequest&);
};

#endif

/* EOF */

//  $Id: property_frame.hxx,v 1.1 2002/06/30 22:32:26 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_PROPERTY_FRAME_HXX
#define HEADER_PINGUS_EDITOR_PROPERTY_FRAME_HXX

#include <ClanLib/gui.h>

namespace Pingus
{
  namespace Editor
  {
    class PropertyFrame : public CL_Frame
    {
    private:

    public:
      PropertyFrame (int width, int height, CL_Component*);
    };
  }
}

#endif

/* EOF */

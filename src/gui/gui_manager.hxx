//  $Id: gui_manager.hxx,v 1.2 2002/07/29 10:44:12 grumbel Exp $
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

#ifndef HEADER_PINGUS_GUI_GUI_MANAGER_HXX
#define HEADER_PINGUS_GUI_GUI_MANAGER_HXX

#include "component.hxx"

namespace GUI
{
  /** interface class which holds all GUI components and displays
      them */
  class GUIManager : public Component
  {
  private:
  public:
    GUIManager () {}
    virtual ~GUIManager () {}
    
    virtual void draw () =0;
    virtual void update (float delta) =0;

    virtual void add (Component*)    =0;
    virtual void remove (Component*) =0;
  };
}

#endif

/* EOF */

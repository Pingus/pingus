//  $Id: gui_screen.cxx,v 1.1 2002/08/01 21:40:01 grumbel Exp $
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

#include "gui/gui_manager.hxx"
#include "gui_screen.hxx"

using namespace GUI;

GUIScreen::GUIScreen ()
  : gui_manager (new GUIManager ())
{
}

GUIScreen::~GUIScreen ()
{
  delete gui_manager;
}

  /** Draw this screen */
void
GUIScreen::draw ()
{
  gui_manager->draw ();
}

  /** Pass a delta to the screen */
void
GUIScreen::update (const GameDelta& delta)
{
  gui_manager->update (delta);
}

/* EOF */

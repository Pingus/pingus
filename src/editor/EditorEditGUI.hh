//  $Id: EditorEditGUI.hh,v 1.1 2000/09/20 15:12:39 grumbel Exp $
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

#ifndef EDITOREDITGUI_HH
#define EDITOREDITGUI_HH

#include "
#include "EditorObj.hh"

class EditorEditGUI
{
private:

public:
  static void edit_obj(EditorObj*);

  static void edit_psm_obj(PSMObj*);
  static void edit_hotspot_obj(HotspotObj*);
  static void edit_entrance_obj(EntranceObj*);
  static void edit_exit_obj(ExitObj*);
};

#endif

/* EOF */

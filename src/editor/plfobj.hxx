//  $Id: plfobj.hxx,v 1.12 2002/09/27 11:26:45 torangan Exp $
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

#ifndef HEADER_PINGUS_EDITOR_PLFOBJ_HXX
#define HEADER_PINGUS_EDITOR_PLFOBJ_HXX

#include "../exit_data.hxx"
#include "sprite_editorobj.hxx"

class ExitObj : public SpriteEditorObj,
		public ExitData
{
public:
  ExitObj (const ExitData&);

  EditorObj* duplicate();
  void write_xml(std::ostream& xml) { ExitData::write_xml (xml); }
  std::string  status_line();
  
private:
  ExitObj (const ExitObj&);
  ExitObj& operator= (const ExitObj&);
};

#endif

/* EOF */


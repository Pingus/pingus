//  $Id: status_line.hxx,v 1.4 2002/07/01 16:31:40 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_STATUS_LINE_HXX
#define HEADER_PINGUS_EDITOR_STATUS_LINE_HXX

#include <list>

class CL_Font;
class EditorObj;
class EditorView;

///
class StatusLine
{
private:
  CL_Font* font;
  const std::list<EditorObj*>* current_objs;
public:
  StatusLine();
  ~StatusLine();
  
  void set_current_objs(const std::list<EditorObj*>* c_objs);
  void draw(EditorView * view);
};

#endif

/* EOF */



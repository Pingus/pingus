//  $Id: status_line.hxx,v 1.1 2002/06/12 19:11:31 grumbel Exp $
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

#ifndef STATUSLINE_HH
#define STATUSLINE_HH

#include <list>

class CL_Font;
class EditorObj;
class EditorView;

///
class StatusLine
{
private:
  ///
  CL_Font* font;
  ///
  std::list<boost::shared_ptr<EditorObj> >* current_objs;
public:
  ///
  StatusLine();
  ///
  ~StatusLine();
  
  ///
  void set_current_objs(std::list<boost::shared_ptr<EditorObj> >* c_objs);
  ///
  void draw(EditorView * view);
};

#endif

/* EOF */



//  $Id: EditorObjGroup.cc,v 1.4 2000/11/14 22:22:56 grumbel Exp $
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

#include "EditorObjGroup.hh"

/******************/
/* EditorObjGroup */
/******************/

EditorObjGroup::EditorObjGroup()
{
}

EditorObjGroup::~EditorObjGroup()
{
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      delete *i;
    }
}

/** Move the object to the given coordinates */
void 
EditorObjGroup::set_position(int new_x_pos, int new_y_pos)
{
  pos.x_pos = new_x_pos;
  pos.y_pos = new_y_pos;

  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->set_position(new_x_pos, new_y_pos);
    }
}

void 
EditorObjGroup::set_position_offset(int x_pos_add, int y_pos_add, int z_pos_add)
{
  pos.x_pos += x_pos_add;
  pos.y_pos += y_pos_add;
  pos.z_pos += z_pos_add;

  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->set_position_offset(x_pos_add, y_pos_add, z_pos_add);
    }
}

/** Draw the object */
void
EditorObjGroup::draw_offset(int x_offset, int y_offset)
{
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->draw_offset(x_offset, y_offset);
    }
}

/** Draw the caputre rectangle around the object */
void
EditorObjGroup::draw_mark_offset(int x_offset, int y_offset, EditorObj::Color* arg_color)
{
  //std::cout << "Group..." << std::endl;
  Color tmp_color = mark_color;
  Color color(1.0, 0.0, 1.0);

  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->draw_mark_offset(x_offset, y_offset, &color);
    }

  mark_color = tmp_color;
  EditorObj::draw_mark_offset(x_offset, y_offset);
}

bool
EditorObjGroup::mouse_over(int x_offset, int y_offset)
{
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      if ((*i)->mouse_over(x_offset, y_offset))
	{
	  return true;
	}
    }
  return false;
}

bool
EditorObjGroup::is_in_rect(int x1, int y1, int x2, int y2)
{
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      if ((*i)->is_in_rect(x1, y1, x2, y2))
	{
	  return true;
	}
    }
  return false;
}

void
EditorObjGroup::push_back(EditorObj* obj)
{
  // Updating the width/height and x_pos/y_pos of the object group
  if (!objs.empty ())
    {
      if (pos.x_pos > obj->get_x_pos())
	pos.x_pos = obj->get_x_pos();
      if (pos.y_pos > obj->get_y_pos())
	pos.y_pos = obj->get_y_pos();

      if ((pos.x_pos + width) < (obj->get_x_pos() + obj->get_width()))
	width = (obj->get_x_pos() + obj->get_width()) - pos.x_pos;
      if ((pos.y_pos + height) < (obj->get_y_pos() + obj->get_height()))
	height = (obj->get_y_pos() + obj->get_height()) - pos.y_pos;
    }
  else
    {
      pos.x_pos = obj->get_x_pos ();
      pos.y_pos = obj->get_y_pos ();
      width = obj->get_width ();
      height = obj->get_height ();
    }

  objs.push_back(obj);
}

list<EditorObj*>* 
EditorObjGroup::get_objs()
{
  return &objs;
}

void
EditorObjGroup::save(std::ofstream* plf, std::ofstream* psm)
{
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->save(plf, psm);
    }
}

///
void
EditorObjGroup::save_xml(std::ofstream* xml)
{
  (*xml) << "<group>\n";
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->save_xml(xml);
    }
  (*xml) << "</group>\n" << std::endl;
}

EditorObj* 
EditorObjGroup::duplicate()
{
  EditorObjGroup* editor_obj = new EditorObjGroup();
  
  for(list<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      EditorObj* obj = (*i)->duplicate();
      if (obj)
	editor_obj->objs.push_back(obj);
    }
  return editor_obj;
}

/* EOF */

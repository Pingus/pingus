//  $Id: EditorObjGroup.cc,v 1.7 2001/04/21 10:55:16 grumbel Exp $
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
  /*
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      delete *i;
    }
  */
}

/** Move the object to the given coordinates */
void 
EditorObjGroup::set_position(int new_x_pos, int new_y_pos)
{
  position->x = new_x_pos;
  position->y = new_y_pos;

  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->set_position(new_x_pos, new_y_pos);
    }
}

void 
EditorObjGroup::set_position_offset(int x_pos_add, int y_pos_add, int z_pos_add)
{
  position->x += x_pos_add;
  position->y += y_pos_add;
  position->z += z_pos_add;

  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->set_position_offset(x_pos_add, y_pos_add, z_pos_add);
    }
}

/** Draw the object */
void
EditorObjGroup::draw_offset(CL_Vector offset, float zoom)
{
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->draw_offset(offset, zoom);
    }
}

/** Draw the caputre rectangle around the object */
void
EditorObjGroup::draw_mark_offset(int x_offset, int y_offset, EditorObj::Color* arg_color)
{
  //std::cout << "Group..." << std::endl;
  Color tmp_color = mark_color;
  Color color(1.0, 0.0, 1.0);

  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
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
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
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
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
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
EditorObjGroup::push_back(boost::shared_ptr<EditorObj> obj)
{
  // Updating the width/height and x_pos/y_pos of the object group
  if (!objs.empty ())
    {
      if (position->x > obj->get_x_pos())
	position->x = obj->get_x_pos();
      if (position->y > obj->get_y_pos())
	position->y = obj->get_y_pos();

      if ((position->x + width) < (obj->get_x_pos() + obj->get_width()))
	width = int((obj->get_x_pos() + obj->get_width()) - position->x);
      if ((position->y + height) < (obj->get_y_pos() + obj->get_height()))
	height = int((obj->get_y_pos() + obj->get_height()) - position->y);
    }
  else
    {
      position->x = obj->get_x_pos ();
      position->y = obj->get_y_pos ();
      width = obj->get_width ();
      height = obj->get_height ();
    }

  objs.push_back(obj);
}

std::list<boost::shared_ptr<EditorObj> >* 
EditorObjGroup::get_objs()
{
  return &objs;
}

void
EditorObjGroup::save(std::ofstream* plf, std::ofstream* psm)
{
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
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

  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->save_xml(xml);
    }
  (*xml) << "</group>\n" << std::endl;
}

boost::shared_ptr<EditorObj> 
EditorObjGroup::duplicate()
{
  boost::shared_ptr<EditorObjGroup> editor_obj(new EditorObjGroup());
  
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      boost::shared_ptr<EditorObj> obj = (*i)->duplicate();
      if (obj.get())
	editor_obj->objs.push_back(obj);
    }
  return editor_obj;
}

/* EOF */

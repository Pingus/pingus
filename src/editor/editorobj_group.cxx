//  $Id: editorobj_group.cxx,v 1.6 2002/09/28 11:52:23 torangan Exp $
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

#include <iostream>
#include "editorobj_group.hxx"

/******************/
/* EditorObjGroup */
/******************/

EditorObjGroup::EditorObjGroup()
{
}

EditorObjGroup::EditorObjGroup(const std::vector<EditorObj*>& arg_objs)
{
  for(std::vector<EditorObj*>::const_iterator i = arg_objs.begin(); i != arg_objs.end (); ++i)
    {
      EditorObj* new_obj = (*i)->duplicate ();
      if (new_obj)
	objs.push_back (new_obj);
      else
	{
	  std::cout << "EditorObjGroup: Duplicate not implemented" << std::endl;
	}
    }
}

EditorObjGroup::~EditorObjGroup()
{
}

void 
EditorObjGroup::set_position_offset(const Vector& offset)
{
  upper_left_corner += offset;
  for(std::vector<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->set_position_offset(offset);
    }
}

/** Draw the object */
void
EditorObjGroup::draw (EditorNS::EditorView * view)
{
  for(std::vector<EditorObj*>::iterator i = objs.begin(); i != objs.end(); ++i)
    {
      (*i)->draw (view);
    }
}

float 
EditorObjGroup::get_z_pos()
{
  /*for(std::vector<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
    }*/
  // FIXME: Hack
  return 100;
}

/** Draw the caputre rectangle around the object */
void
EditorObjGroup::draw_mark (EditorNS::EditorView * view)
{
  for(std::vector<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      (*i)->draw_mark (view);
    }
}

void
EditorObjGroup::add (EditorObj* obj)
{
  // Updating the width/height and x_pos/y_pos of the object group
  /*
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
  */

  EditorObj* new_obj = obj->duplicate ();
  if (new_obj)
    objs.push_back(new_obj);
}

std::vector<EditorObj*>* 
EditorObjGroup::get_objs()
{
  return &objs;
}

///
void
EditorObjGroup::write_xml(std::ostream& xml)
{
  xml << "<group>\n";

  for(std::vector<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->write_xml(xml);
    }
  xml << "</group>\n" << std::endl;
}

EditorObj*
EditorObjGroup::duplicate()
{
  EditorObjGroup* editor_obj = new EditorObjGroup();
  
  for(std::vector<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      EditorObj* obj = (*i)->duplicate();
      if (obj)
	editor_obj->objs.push_back(obj);
    }
  return editor_obj;
}

bool 
EditorObjGroup::is_over(const Vector& pos)
{
  for(std::vector<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      if ((*i)->is_over (pos))
	return true;
    }
  return false;
}

bool 
EditorObjGroup::is_in_rect(const CL_Rect& rect)
{
  for(std::vector<EditorObj*>::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      if ((*i)->is_in_rect (rect))
	return true;
    }
  return false;
}

/* EOF */

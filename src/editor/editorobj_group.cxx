//  $Id: editorobj_group.cxx,v 1.1 2002/06/12 19:11:31 grumbel Exp $
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

#include <fstream>
#include "editorobj_group.hxx"

/******************/
/* EditorObjGroup */
/******************/

EditorObjGroup::EditorObjGroup()
{
}

EditorObjGroup::~EditorObjGroup()
{
}

void 
EditorObjGroup::set_position_offset(const CL_Vector& offset)
{
  upper_left_corner += offset;
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->set_position_offset(offset);
    }
}

/** Draw the object */
void
EditorObjGroup::draw (EditorView * view)
{
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      (*i)->draw (view);
    }
}

float 
EditorObjGroup::get_z_pos()
{
  /*for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
    }*/
  // FIXME: Hack
  return 100;
}

/** Draw the caputre rectangle around the object */
void
EditorObjGroup::draw_mark (EditorView * view)
{
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      (*i)->draw_mark (view);
    }
}

void
EditorObjGroup::add (boost::shared_ptr<EditorObj> obj)
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
  objs.push_back(obj);
}

std::list<boost::shared_ptr<EditorObj> >* 
EditorObjGroup::get_objs()
{
  return &objs;
}

///
void
EditorObjGroup::write_xml(std::ofstream* xml)
{
  (*xml) << "<group>\n";

  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      i++)
    {
      (*i)->write_xml(xml);
    }
  (*xml) << "</group>\n" << std::endl;
}

boost::shared_ptr<EditorObj> 
EditorObjGroup::duplicate()
{
  EditorObjGroup* editor_obj = new EditorObjGroup();
  
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      boost::shared_ptr<EditorObj> obj = (*i)->duplicate();
      if (obj.get())
	editor_obj->objs.push_back(obj);
    }
  return boost::shared_ptr<EditorObj> (editor_obj);
}

bool 
EditorObjGroup::is_over(const CL_Vector& pos)
{
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
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
  for(std::list<boost::shared_ptr<EditorObj> >::iterator i = objs.begin();
      i != objs.end();
      ++i)
    {
      if ((*i)->is_in_rect (rect))
	return true;
    }
  return false;
}

/* EOF */

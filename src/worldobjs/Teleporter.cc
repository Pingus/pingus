//  $Id: Teleporter.cc,v 1.33 2002/06/01 18:05:37 torangan Exp $
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

#include "../World.hh"
#include "../PingusResource.hh"
#include "../PinguHolder.hh"
#include "../XMLhelper.hh"
#include "../editor/EditorView.hh"
#include "Teleporter.hh"

TeleporterData::TeleporterData (const TeleporterData& data) : WorldObjData(data)
{
  pos = data.pos;
  target_pos = data.target_pos;
}

void 
TeleporterData::write_xml(std::ofstream* xml)
{
  (*xml) << "  <worldobj type=\"teleporter\">";
  XMLhelper::write_position_xml (xml, pos);
  (*xml) << "    <target>" << std::endl;
  XMLhelper::write_position_xml (xml, target_pos);
  (*xml) << "    </target>" << std::endl;
  (*xml) << "  </worldobj>" << std::endl;
}

boost::shared_ptr<WorldObjData>
TeleporterData::create(xmlDocPtr doc, xmlNodePtr cur)
{
  TeleporterData* data = new TeleporterData ();
  
  cur = cur->children;
  
  while (cur != NULL)
    {
      if (xmlIsBlankNode(cur)) 
	{
	  cur = cur->next;
	  continue;
	}

      if (strcmp((char*)cur->name, "position") == 0)
	{
	  data->pos = XMLhelper::parse_vector (doc, cur);
	}
      else if (strcmp((char*)cur->name, "target") == 0)
	{
	  xmlNodePtr ncur = cur->children;

	  if (xmlIsBlankNode(ncur)) ncur = ncur->next;
	    
	  if (ncur != NULL)
	    data->target_pos = XMLhelper::parse_vector (doc, ncur);
	  else
	    std::cout << "TeleporterData::create (): <target> is empty" << std::endl;
	}
      else
	{
	  std::cout << "TeleportData::create (): Unhandled " << cur->name << std::endl;
	}

      cur = cur->next;
    }
  return boost::shared_ptr<WorldObjData> (data);
}

boost::shared_ptr<WorldObj> 
TeleporterData::create_WorldObj ()
{
  //std::cout << "TeleportData::create_WorldObj () not implemented" << std::endl;
  return boost::shared_ptr<WorldObj> (new Teleporter (*this));
}

std::list<boost::shared_ptr<EditorObj> > 
TeleporterData::create_EditorObj ()
{
  std::cout << "TeleportData::create_EditorObj () " << std::endl;
  std::list<boost::shared_ptr<EditorObj> > objs;
  
  EditorTeleporterObj*       teleporter = new EditorTeleporterObj (*this);
  EditorTeleporterTargetObj* teleporter_target = new EditorTeleporterTargetObj (teleporter);

  objs.push_back (boost::shared_ptr<EditorObj> (teleporter));
  objs.push_back (boost::shared_ptr<EditorObj> (teleporter_target));

  std::cout << "TeleportData::create_EditorObj (): done" << std::endl;

  return objs;
}

/**************/
/* Teleporter */
/**************/

Teleporter::Teleporter (const TeleporterData& data)
  : sprite ("teleporter", "worldobjs", 20.0f, Sprite::NONE, Sprite::ONCE),
    target_sprite ("teleportertarget", "worldobjs", 20.0f, Sprite::NONE, Sprite::ONCE)
{  
  sprite.set_align_center_bottom ();
  target_sprite.set_align_center ();

  //FIXME: we need a Sprite::set_frame()

  pos = data.pos;
  target_pos = data.target_pos;

  std::cout << "Teleporter: pos: " << pos.x << " "  << pos.y << " " << pos.z << std::endl;
}

void 
Teleporter::draw_offset (int x_of, int y_of, float /*s*/)
{
  //std::cout << "Teleporter::draw_offset ()" << std::endl;
  //view->draw (sur, pos);
  sprite.put_screen (pos + CL_Vector(x_of, y_of));
  target_sprite.put_screen (target_pos + CL_Vector(x_of, y_of));
}

void 
Teleporter::update (float delta)
{
  sprite.update (delta);
  target_sprite.update (delta);

  PinguHolder* holder = world->get_pingu_p();

  for (PinguIter pingu = holder->begin (); pingu != holder->end (); pingu++)
    {
      if ((*pingu)->get_x() > pos.x - 3 && (*pingu)->get_x() < pos.x + 3
	  && (*pingu)->get_y() < pos.y && (*pingu)->get_y() > pos.y - 52)
	{
	  (*pingu)->set_pos (int(target_pos.x), int(target_pos.y));
	  sprite.reset ();
	  target_sprite.reset ();
	}
    }
}

/********************/
/* EditorTeleporter */
/********************/

EditorTeleporterObj::EditorTeleporterObj (const TeleporterData& data)
  : SpriteEditorObj ("teleporter", "worldobjs", pos)
{
  sprite.set_align_center_bottom ();

  pos        = data.pos;
  target_pos = data.target_pos;
}

boost::shared_ptr<EditorObj> 
EditorTeleporterObj::duplicate()
{
  std::cout << "EditorTeleporterObj::duplicate(): not implemented" << std::endl;
  return boost::shared_ptr<EditorObj> ();
}

std::list<boost::shared_ptr<EditorObj> > 
EditorTeleporterObj::create (const TeleporterData& data)
{
  std::list<boost::shared_ptr<EditorObj> > objs;

  //std::cout << "EditorTeleporterObj: " << tdata << " - " << tdata->target_pos << std::endl;

  EditorTeleporterObj* teleporter=new EditorTeleporterObj (data);
  EditorTeleporterTargetObj* teleporter_target=new EditorTeleporterTargetObj (teleporter);

  objs.push_back (boost::shared_ptr<EditorObj>(teleporter));
  objs.push_back (boost::shared_ptr<EditorObj>(teleporter_target));

  return objs;
}

std::list<boost::shared_ptr<EditorObj> >
EditorTeleporterObj::create (const CL_Vector& pos)
{
  TeleporterData data;

  std::cout << "EditorTeleporterObj: creating..." << std::endl;

  data.pos = pos;
  data.target_pos.x = pos.x + 50;
  data.target_pos.y = pos.y + 50;

  return data.create_EditorObj ();
}

void
EditorTeleporterObj::draw (boost::dummy_ptr<EditorView> view)
{
  //std::cout << "Drawing line" << std::endl;
  view->draw_line (int(pos.x), 
		   int(pos.y),
		   int(target_pos.x), 
		   int(target_pos.y),
		   0.0, 1.0, 0.0, 0.5);
  SpriteEditorObj::draw (view);
}

void
EditorTeleporterObj::save_xml (std::ofstream* xml)
{
  // Before we write down the xml stuff, we need to get the positions
  // of the objects
  //  TeleporterData::target_pos = target->get_position ();
  this->write_xml (xml);
}

std::string
EditorTeleporterObj::status_line()
{
  // FIXME: replace with string streams
  char str[1024];
  sprintf (str, "Teleporter - %f %f %f", 
	   pos.x, pos.y, pos.z);
  return str;
}

/*****************************/
/* EditorTeleporterTargetObj */
/*****************************/

EditorTeleporterTargetObj::EditorTeleporterTargetObj (EditorTeleporterObj* arg_teleporter)
  : SpriteEditorObj ("teleporter2", "worldobjs", arg_teleporter->get_target_pos_ref ()),
  teleporter (arg_teleporter)
{
  sprite.set_align_center();
}

std::string
EditorTeleporterTargetObj::status_line()
{
  char str[1024];
  sprintf (str, "TeleporterTarget - %f %f %f", 
	   pos_ref.x, pos_ref.y, pos_ref.z);
  return str;
}

/* EOF */

//  $Id: EditorWorldObj.cc,v 1.5 2000/11/17 19:09:21 grumbel Exp $
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

#include "../worldobjs/ConveyorBelt.hh"
#include "../worldobjs/IceBlock.hh"
#include "../worldobjs/Teleporter.hh"
#include "../PingusResource.hh"
#include "EditorWorldObj.hh"

EditorWorldObj::EditorWorldObj ()
{
  surf = PingusResource::load_surface ("misc/404", "core");
}
/*
list<EditorObj*> 
EditorWorldObj::create (WorldObjData* data)
{
  if (dynamic_cast <TeleporterData*>(data))
    {
      return EditorTeleporterObj::create (data);
    }
  else if (dynamic_cast <IceBlockData*>(data))
    {
      return EditorIceBlockObj::create (data);
    }
  else if (dynamic_cast <ConveyorBelt*>(data))
    {
      return EditorConveyorBeltObj::create (data);
    }
  else
    {
      std::cout << "EditorWorldObj: Warrning! Unsupported object type!" << std::endl;
      return list<EditorObj*>();
    }
}
*/
EditorObj* 
EditorWorldObj::duplicate()
{
  return 0;
}
  
void 
EditorWorldObj::save_xml(std::ofstream* xml)
{
  
} 

/* EOF */

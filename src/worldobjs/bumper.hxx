//  $Id: bumper.hxx,v 1.7 2002/10/01 19:53:45 grumbel Exp $
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

#ifndef HEADER_PINGUS_WORLDOBJS_BUMPER_HXX
#define HEADER_PINGUS_WORLDOBJS_BUMPER_HXX

#include "../worldobj.hxx"

namespace WorldObjsData {
class BumperData;
} // namespace WorldObjsData

class Pingu;

namespace WorldObjs {

class Bumper : public WorldObj
{
private:
  WorldObjsData::BumperData* const data;
  bool upwards;
  int  count;
    
public:
  Bumper (const WorldObjsData::BumperData& data_);
  ~Bumper ();

  float get_z_pos () const;
    
  void draw (GraphicContext& gc);
  void on_startup();
  void update ();

private:    
  void catch_pingu (Pingu* pingu);

  Bumper (const Bumper&);
  Bumper& operator= (const Bumper&);
};

} // namespace WorldObjs

#endif

/* EOF */

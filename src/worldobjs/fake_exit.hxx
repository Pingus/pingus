//  $Id: fake_exit.hxx,v 1.6 2002/10/01 19:53:46 grumbel Exp $
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

#ifndef HEADER_PINGUS_TRAPS_FAKE_EXIT_HXX
#define HEADER_PINGUS_TRAPS_FAKE_EXIT_HXX

#include "../worldobj.hxx"

class Pingu;

namespace WorldObjsData {
class FakeExitData;
} // namespace WorldObjsData

namespace WorldObjs {

/** The FakeExit is an object that looks similar to the traditional
    IceExit, but is really a trap that smashed all pingus that try to
    enter it. */
class FakeExit : public WorldObj
{
private:
  WorldObjsData::FakeExitData* const data;
  bool smashing;
    
public:
  FakeExit (const WorldObjsData::FakeExitData& data_);
  ~FakeExit ();

  float get_z_pos () const;
    
  void draw (GraphicContext& gc);

  void update ();

private:
  void catch_pingu (Pingu*);
    
  FakeExit (const FakeExit&);
  FakeExit& operator= (const FakeExit&);
};

} // namespace WorldObjs

#endif

/* EOF */

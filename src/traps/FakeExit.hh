//  $Id: FakeExit.hh,v 1.9 2002/06/07 20:35:14 torangan Exp $
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

#ifndef FAKE_EXIT_HH
#define FAKE_EXIT_HH

#include "../Trap.hh"

class TrapData;

///
class FakeExit : public Trap
{
private:
  bool smashing;
public:
  FakeExit(const TrapData& data);
  virtual ~FakeExit();
  
  void update(float delta);
  void catch_pingu(boost::shared_ptr<Pingu>);
};

#endif

/* EOF */

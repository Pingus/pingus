//  $Id: PLFObj.hh,v 1.1 2000/02/04 23:45:19 mbn Exp $
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

#ifndef PLFOBJ_HH
#define PLFOBJ_HH

#include "../Direction.hh"
#include "../Liquid.hh"
#include "EditorObj.hh"

class PLFObj : public EditorObj
{
private:
  
public:
  PLFObj();
  virtual ~PLFObj();

  virtual void save(ofstream* plf, ofstream* psm) = 0;
};

class HotspotObj : public PLFObj
{
private:
  int   speed;
  float para;
public:
  HotspotObj(hotspot_data);
  ~HotspotObj();
  EditorObj* duplicate();
  void save(ofstream* plf, ofstream* psm);
  string obj_type() { return "Hotspot"; };
};

class EntranceObj : public PLFObj
{
private:
  string type;
  int release_rate;
  entrance_data::Direction direction;
public:
  EntranceObj(entrance_data);
  ~EntranceObj();
  EditorObj* duplicate();
  void save(ofstream* plf, ofstream* psm);  
  string obj_type() { return "Entrance"; };
};

class ExitObj : public PLFObj
{
private:
public:
  ExitObj(exit_data);
  ~ExitObj();
  EditorObj* duplicate();
  void save(ofstream* plf, ofstream* psm);  
  string obj_type() { return "Exit"; };
};


class TrapObj : public PLFObj
{
private:
  string name;
  int    frame;
public:
  TrapObj(trap_data);
  ~TrapObj();
  EditorObj* duplicate();
  void save(ofstream* plf, ofstream* psm);  
  void draw_offset(int x_offset, int y_offset);
  string obj_type() { return "Trap"; };
};

class LiquidObj : public PLFObj
{
private:
  int width;
  int speed;
  AnimCounter counter;
public:
  LiquidObj(liquid_data);
  ~LiquidObj();
  EditorObj* duplicate();
  void draw_offset(int x_offset, int y_offset);
  void draw_mark_offset(int x_offset, int y_offset);
  bool mouse_over(int, int);
  void save(ofstream* plf, ofstream* psm);
  string obj_type() { return "Liquid"; };
};

#endif

/* EOF */



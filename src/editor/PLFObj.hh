//  $Id: PLFObj.hh,v 1.14 2001/04/12 09:02:24 grumbel Exp $
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

#include "../WorldObjData.hh"
#include "../Direction.hh"
#include "../Liquid.hh"
#include "EditorObj.hh"

///
class PLFObj : public EditorObj
{
private:
  
public:
  ///
  PLFObj();
  ///
  virtual ~PLFObj();

  ///
  virtual void save(std::ofstream* plf, std::ofstream* psm) = 0;
  ///
  virtual void save_xml(std::ofstream* xml) = 0;
  ///
  virtual std::string  status_line();
};

///
class HotspotObj : public PLFObj
{
private:
  ///
  int   speed;
  ///
  float para;
public:
  ///
  HotspotObj(HotspotData);
  ///
  ~HotspotObj();
  ///
  boost::shared_ptr<EditorObj> duplicate();
  ///
  void save(std::ofstream* plf, std::ofstream* psm);
  ///
  void save_xml(std::ofstream* xml);
  ///
  std::string  status_line();
};

///
class EntranceObj : public PLFObj
{
private:
  ///
  std::string type;
  ///
  int release_rate;
  ///
  int owner_id;
  ///
  EntranceData::EntranceDirection direction;
public:
  ///
  EntranceObj(EntranceData);
  ///
  ~EntranceObj();
  ///
  boost::shared_ptr<EditorObj> duplicate();
  ///
  void save(std::ofstream* plf, std::ofstream* psm);  
  ///
  void save_xml(std::ofstream* xml);
  ///
  std::string status_line();
};

///
class ExitObj : public PLFObj
{
private:
public:
  ///
  ExitObj(ExitData);
  ///
  ~ExitObj();
  ///
  boost::shared_ptr<EditorObj> duplicate();
  ///
  void save_xml(std::ofstream* xml);
  ///
  void save(std::ofstream* plf, std::ofstream* psm);  
  ///
  std::string  status_line();
};


///
class TrapObj : public PLFObj
{
private:
  ///
  std::string type;
  ///
  int    frame;
public:
  ///
  TrapObj(TrapData);
  ///
  ~TrapObj();
  ///
  boost::shared_ptr<EditorObj> duplicate();
  ///
  void save(std::ofstream* plf, std::ofstream* psm);  
  ///
  void save_xml(std::ofstream* xml);
  ///
  void draw_offset(int x_offset, int y_offset);
};

///
class LiquidObj : public PLFObj
{
private:
  ///
  int width;
  ///
  int speed;
  ///
  AnimCounter counter;
public:
  ///
  LiquidObj(LiquidData);
  ///
  LiquidObj(const LiquidObj& data);
  ///
  ~LiquidObj();
  ///
  boost::shared_ptr<EditorObj> duplicate();
  ///
  void draw_offset(int x_offset, int y_offset);
  ///
  void draw_mark_offset(int x_offset, int y_offset);
  ///
  bool mouse_over(int, int);
  ///
  void save(std::ofstream* plf, std::ofstream* psm);
  ///
  void save_xml(std::ofstream* xml);
  ///
  std::string  status_line();
};

#endif

/* EOF */



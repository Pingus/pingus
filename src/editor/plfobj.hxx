//  $Id: plfobj.hxx,v 1.2 2002/06/24 22:52:58 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_PLFOBJ_HXX
#define HEADER_PINGUS_EDITOR_PLFOBJ_HXX

#include "../liquid_data.hxx"
#include "../entrance_data.hxx"
#include "../exit_data.hxx"
#include "../trap_data.hxx"
#include "sprite_editorobj.hxx"


using namespace std;

///
class EntranceObj : public SpriteEditorObj,
		    public EntranceData
{
private:
public:
  EntranceObj(const EntranceData&);

  boost::shared_ptr<EditorObj> duplicate();
  void write_xml(std::ofstream* xml) { EntranceData::write_xml (xml); }
  std::string status_line();
};

///
class ExitObj : public SpriteEditorObj,
		public ExitData
{
private:
public:
  ExitObj(const ExitData&);

  boost::shared_ptr<EditorObj> duplicate();
  void write_xml(std::ofstream* xml) { ExitData::write_xml (xml); }
  std::string  status_line();
};


///
class TrapObj : public SpriteEditorObj,
		protected TrapData
{
private:
  int frame;
public:
  TrapObj(const TrapData&);

  boost::shared_ptr<EditorObj> duplicate();
  void write_xml(std::ofstream* xml) { TrapData::write_xml (xml); }
  void draw (EditorView * view);
  std::string  status_line();
};

///
class LiquidObj : public SpriteEditorObj,
		  protected LiquidData
{
private:
public:
  LiquidObj(const LiquidData& data);

  boost::shared_ptr<EditorObj> duplicate();
  void draw (EditorView * view);
  void draw_mark (EditorView * view);
  void save(std::ofstream* plf, std::ofstream* psm);
  void write_xml(std::ofstream* xml) { LiquidData::write_xml (xml); }
  std::string  status_line();

  int get_width () { return  sprite.get_width () * width; }

  void make_larger ();
  void make_smaller ();
};

#endif

/* EOF */



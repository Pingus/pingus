//  $Id: plf.hxx,v 1.7 2002/09/16 20:31:09 grumbel Exp $
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

#ifndef HEADER_PINGUS_PLF_HXX
#define HEADER_PINGUS_PLF_HXX

#include <map>
#include "action_data.hxx"
#include "worldobjsdata/groundpiece_data.hxx"

class WorldObjData;

/** The Pingus Level File
    
    The PLF class holds all object information, which are needed to
    build a level, like the levelname, description, number of pingus,
    etc. */
class PLF 
{
protected:
  std::map<std::string, std::string> levelname;

  /** The name of the file from were the current level was loaded */
  std::string filename;

  std::map<std::string, std::string> description;
  
  /** Author of this level */
  std::string author;

  ResDescriptor music;

  /** Start x-position of the camera */
  int  start_x_pos;
  /** Start y-position of the camera */
  int  start_y_pos;

  /** width of the level */
  int  width;

  /** height of the level */
  int  height;

  /** Timelimit for this level */
  int  max_time;

  int  release_rate;
  int  number_of_pingus;
  int  number_to_save;

  /** the difficulty of the level, difficulty should be between 0-99

   0 - works without interaction
  10 - tutorial, requires a single click
  20 - requires multiple clicks, but solution is obvious
  30 - ...
  40 - ...
  50 - medium level, requires some thinking
  70 - hard level, requires quite a lot of thinking
  90 - requires knowlege of many special combinations and other things
  99 - extreme hard, requires absolute precision and no errors (basically unplayable)
   */
  int  difficulty;

  /** Internal comment about this level, might include a solution hint
      or other things which are needed to maintain this level. */
  std::string comment;

  /** levels flagged with playable=false are not playable or havn't been yet tested */
  bool playable;
  
  std::vector<ActionData>       actions;
  std::vector<WorldObjsData::GroundpieceData>  groundpieces;

  /** FIXME: PLF should probally become and interface only, it currently is a
      bit overfull */
  std::vector<WorldObjData*> worldobjs_data;

public:
  PLF();
  virtual ~PLF();

  /** Returns the ResDescriptor where the music for this level can be
      found. The ResDescriptor points normaly to a file. */
  ResDescriptor get_music(void);

  /** Returns the start position for the camera */
  int         get_startx(void);

  /** Returns the start position for the camera */
  int         get_starty(void);

  /** Returns the width of the level */
  int         get_width(void);

  /** Returns the height of the level */
  int         get_height(void);

  /** Returns the number of Pingus, which are going to be released in
      this level. */
  int         get_pingus(void);
  
  /** Returns the time you have to complete a level */
  int         get_time(void);

  /** Returns the difficulty of the current level */
  int         get_difficulty();

  /** Returns if the level is playable */
  bool        get_playable();

  /** Returns the maintainer internal comment */
  std::string get_comment ();

  /** Returns the number of pingus you need to save in this level. */
  int         get_number_to_save();

  /** Returns the description of the level, which fit to the current
      language */
  std::map<std::string, std::string> get_description();

  /** Returns the name of the current level, {\em not} the level file name. */
  std::map<std::string, std::string> get_levelname();

  /** Returns the name of the plf file. */
  std::string get_filename();

  /** Returns the name of the author, who creates this level */
  std::string get_author();

  std::vector<ActionData>      get_actions(void);
  std::vector<WorldObjsData::GroundpieceData> get_groundpieces(void);

  /** Return a access to the worldobj_data, the caller must not delete
      anything in this vector */
  std::vector<WorldObjData*> get_worldobjs_data ();

  /** Creates a PLF (XMLPLF or PLFPLF, depending on the file
      extension) from a file. The pathname must be complete */
  static PLF* create (const std::string& pathname);
  
private:
  PLF (const PLF&);
  PLF operator= (const PLF&);
};

#endif

/* EOF */

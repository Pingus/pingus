//  $Id: PLFParser.hh,v 1.8 2002/06/08 20:19:53 torangan Exp $
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

#ifndef PLFParser_HH
#define PLFParser_HH

#include <fstream>
#include <string>

/** This gets thrown if a end of file is found inside a PLF file */
class PLFParserEOF {
public:
  PLFParserEOF();
};

/** A parser for PLF files */
class PLFParser
{
private:
  /// The file to parse
  std::ifstream in;               

  /// The last returned atom
  char   last_atom;    
  /// Current line number
  int    lineno; 
  
  bool   eof;
  
  // private functions  
  
  /// Return the next char and to a eof check  
  char   get_char(void);
  
  /// Return the next atom and keep the old one
  char   get_atom(void);
  
  /// Return the next atom (removes all comments)
  char   get_raw_atom(void);  
  
  /// Return the groupname and check for errors
  std::string get_groupname(void);  
  
  /// Return the value identifer
  std::string get_valueid(void);
  
  /// Return the value
  std::string get_value(void);

  /// Return the cast, else ""
  std::string get_cast(void);
  
  /// Jump to the next token, after char
  void   jump_after(char);   

  /// Jump over spaces to the next token
  void   jump(void);

  /// Do a clean shutdown on a syntax error
  void   syntax_error(std::string);

  /// Parse the opened file
  void   parse(void);          

  /// Open the file
  void  open(std::string);

  /** Some virtual functions
      Put the retrieved value in the correct struct */
  virtual void set_value(std::string valueid,
			 std::string cast,
			 std::string value) = 0; 
  ///
  virtual void set_group_start(std::string) = 0;
  ///
  virtual void set_group_end(void) = 0;
public:
  ///
  PLFParser();
  
  /// Close the file
  virtual ~PLFParser();

  /** Init the PLFParser and start parsing
      @param filename The filename which should be parsed */
  void init(std::string filename); 
};

#endif

/* EOF */

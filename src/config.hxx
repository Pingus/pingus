//  $Id: config.hxx,v 1.6 2002/09/27 11:26:43 torangan Exp $
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

#ifndef HEADER_PINGUS_CONFIG_HXX
#define HEADER_PINGUS_CONFIG_HXX

#include "pingus.hxx"
#include <string>
#include <fstream>

class ConfigParser
{
private:
  
  ///
  std::ifstream in;                 /// The file to parse

  char   last_atom;            /// The last returned atom
  int    lineno;               /// Current line number
  bool   eof;
  //  std::string filename;             // The name of the current file

  /// private functions    
  char   get_char(void);       /// Return the next char and to a eof check
  char   get_atom(void);       /// Return the next atom and keep the old one
  char   get_raw_atom(void);   /// Return the next atom (removes all comments)
  std::string get_valueid(void);    /// Return the value identifer
  std::string get_value(void);      /// Return the value
  void   jump_after(char);     /// Jump to the next token, after char
  void   jump(void);           /// Jump over spaces to the next token
  void   syntax_error(std::string); /// Do a clean shutdown on a syntax error
  void   parse(void);          /// Parse the opened file
  void   open(std::string);          // Open the file

  // Some virtual functions
  /// Put the retrieved value in the correct struct
  virtual void set_value(std::string valueid,
			 std::string value) = 0; 

public:
  ///
  ConfigParser();         
  ///
  ConfigParser(std::string);         /// Open the file and parse it
  virtual ~ConfigParser();      /// Close the file
    
  void init(std::string);
  
private:
  ConfigParser (const ConfigParser&);
  ConfigParser& operator= (const ConfigParser&);
};

class Config : public ConfigParser
{
private:
  std::string filename;

  bool str_to_bool(const std::string&);
  int  str_to_int(const std::string&);
  
public:
  Config ();
  Config (std::string);

  void set_value(std::string valueid,
		 std::string value);
		 
private:
  Config (const Config&);
  Config& operator= (const Config&);
};

#endif

/* EOF */

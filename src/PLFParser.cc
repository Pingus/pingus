//  $Id: PLFParser.cc,v 1.5 2000/02/16 23:34:11 grumbel Exp $
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

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>

#include "PLFParser.hh"
#include "PingusError.hh"
#include "globals.hh"

PLFParserEOF::PLFParserEOF()
{
}

PLFParser::PLFParser()
{
}

// Create a PLF object and start parsing the given file.
PLFParser::PLFParser(std::string filename)
{
  init(filename);
}

// Destroy all data
PLFParser::~PLFParser()
{
}

void
PLFParser::init(std::string filename)
{
  // Init local vars
  last_atom = ' ';
  lineno = 1;

  // Start parsing
  open(filename);
  parse();
}

// Open the file and do some error checking.
void
PLFParser::open(std::string filename)
{
  in.open(filename.c_str());
  eof = false;

  if (!in) {
    throw PingusError("Couldn't open: " + filename);
  }
  
  if (verbose > 1)
    std::cout << "Opened plf file successfully" << std::endl;
}

// Return the next char from file and check for eof.
char
PLFParser::get_char(void)
{
  int c;

  if (eof) {
    if (verbose > 1) std::cout << "PLFParser: Result of get_char() will be undefined" << std::endl;
    // throw PingusError("");
  }

  c = in.get();

  if (c == EOF) {
    if (verbose > 1) std::cout << "PLF::get_char(): Found EOF!" << std::endl;
    eof = true;
    // throw PLFParserEOF();
  }

  if (c == '\n')
    ++lineno;

  return c;
}

// Return the next char from file, remove all comments and spaces before.
char
PLFParser::get_raw_atom(void)
{
  char c;
  char temp_c;
  c = get_char();

  // Ignoring "//" comments
  if (c == '/') {
    char ic;
    if ((ic = get_char()) == '/') {
      while(get_char() != '\n'); // Ignoring until EOL
      return get_atom();
    } else {
      in.putback(ic);
    }
  }

  if (c == '/') {
    if ((c = get_char()) == '*') {
      while(true) {
	c = get_char();
	if (c == '*')
	  if (get_char() == '/')
	    return get_atom();
      }
    } else {
      in.putback(c);
    }
  }
  
  if (isspace(c)) {
    temp_c = c;
    while (isspace(c = get_char()));
    in.putback(c);
    if (isspace(last_atom)) 
      return get_atom();
    return temp_c;
  }

  return c;
}

// Return the next atom and keep it.
char
PLFParser::get_atom(void)
{
  last_atom = get_raw_atom();
  
  return last_atom;
}

string
PLFParser::get_groupname(void)
{
  char atom;
  std::string ret_val;
  
  jump();

  while((isalpha(atom = get_atom()) || atom == '_')) 
    {
      ret_val += atom;
    }

  if (eof) 
    return ""; 
      
  if (ret_val.empty())
    syntax_error(string("Invalid group name: Found char: '") + atom + "'");

  return ret_val;
}

// Return the name of the value identiver.
string
PLFParser::get_valueid(void)
{
  std::string ret_val;
  char   atom;
  
  jump();

  while(true) {
    atom = get_atom();
    
    if (atom == '}' && ret_val.empty()) {
      return "}";
    }

    if (isalpha(atom) || atom == '_') {
      ret_val += atom;
    } else if (isspace(atom)) {
      return ret_val;
    } else if (atom == '=') {
      in.putback(atom);
      return ret_val;
    } else {
      syntax_error(string("Unexpected char: '") + atom + "'");
    }
  }  
  
  return ret_val;
}

string
PLFParser::get_value(void)
{
  std::string ret_val;
  char   atom;

  jump();

  while(true) {
    atom = get_atom();

    if (atom == '"' && ret_val == "") {
      while((atom = get_char()) != '"') {
	ret_val += atom;
      }
      return ret_val;
    }

    if (atom == ';') {
      in.putback(atom);
      return ret_val;
    }
   
    if (!isalnum(atom) && atom != '-' && atom != '_') {
      if (isspace(atom)){
	return ret_val;
      } else {
	syntax_error(string("Unexpected char '") + atom + "'");
      }
    }
    
    ret_val += atom;
  }
  
  return ret_val;
}

string
PLFParser::get_cast(void)
{
  std::string ret_val;
  char   atom;
  jump();
  
  atom = get_atom();

  if (atom != '(') {
    in.putback(atom);
    return ret_val;
  } else {
    while(true) {
      atom = get_atom();
      if (atom == ')')
	break;
      ret_val += atom;
    }
  }
  return ret_val;
}

// Jumps to the position of the next token after 'a', no other char's
// then spaces are allowed before 'a'.
void
PLFParser::jump_after(char c)
{
  char atom;
  
  jump();

  atom = get_atom();
  if (isspace(atom) || atom == c) {
    if (atom == c) {
      return;
    } else {
      atom = get_atom();
      if (atom == c)
	return;
    }
  } 
  syntax_error(string("jump_after(): Expected '") + c + "', got '" + atom + "'" );
}

void
PLFParser::jump(void)
{
  char atom;
  atom = get_atom();

  if (isspace(atom)) {
    return;
  } else {
    in.putback(atom);
  }
}

// Print a error message with lineno and filename.
void
PLFParser::syntax_error(std::string error = "")
{
  std::string error_str;
  char tmp[256];

  sprintf(tmp, "%d\n", lineno);
  
  error_str = string("PLF: Syntax Error at line ") + tmp;

  if (error != "")
    error_str += "PLF:" + error + "\n";

  throw PingusError(error_str);
}

// Parse the file and fill all structs with the values.
void
PLFParser::parse(void)
{
  std::string groupname;
  std::string valueid;
  std::string cast;
  std::string value;

  while(!eof) 
    {
      groupname = get_groupname();

      if (eof) break;   

      set_group_start(groupname);
      jump_after('{');
 
      while(true) 
	{
	  valueid = get_valueid();

	  if (valueid == "}")
	    break;
	  
	  jump_after('=');
	  cast    = get_cast();
	  value   = get_value();
	  jump_after(';');
	  
	  if (verbose > 2) 
	    {
	      std::cout << "ValueID: " << valueid << std::endl;
	      std::cout << "Cast: " << cast << std::endl;
	      std::cout << "Value: " << value << std::endl;
	    }
	  set_value(valueid, cast, value);
	}
      set_group_end();
      jump();
    }
  
  /*  catch(PLFParserEOF a) {
    if (verbose > 1) std::cout << "PLF: Catched EOF" << std::endl;
    }*/
}

/* EOF */

//  $Id: PingusStream.hh,v 1.1 2002/06/05 17:51:08 grumbel Exp $
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

#ifndef PINGUSSTREAM_HH
#define PINGUSSTREAM_HH

#include <string>
#include <vector>
#include <iostream>

class MultiplexStreamBuffer
  : public std::streambuf
{
private:
  static const int buffersize;

  std::vector<std::ostream*> out_streams;

  std::string current_line;
  bool continue_last;
  std::vector<std::string> buffer;
  char char_buffer[200];
  
  std::string prefix;

  void put_line (const std::string& line);
public:
  MultiplexStreamBuffer (const std::string& prefix);
  virtual ~MultiplexStreamBuffer ();

  int overflow (int c);  
  int sync ();

  void add (std::ostream& s);
};

class MultiplexStream
  : public std::ostream
{
private:
  MultiplexStreamBuffer buffer;
  
public:
  MultiplexStream (const std::string& prefix);
  virtual ~MultiplexStream ();

  void add (std::ostream& s);
};

#endif

/* EOF */

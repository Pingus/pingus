//  $Id: PingusStream.hh,v 1.2 2002/06/06 13:56:48 torangan Exp $
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


class DebugStream
  : public std::ostream
{

private:

  class Buffer
    : public std::streambuf
  {
  private:

    // unnessecary complex in this case
    // static const int buffersize;
    enum { buffersize = 200 };

    std::vector<std::ostream*> out_streams;

    char char_buffer[200];
    std::string prefix;
  
  public:

    Buffer (const std::string& prefix);
    virtual ~Buffer ();

    int overflow (int c);  
    int sync ();

    void add (std::ostream& s);
    void set_prefix (const std::string & prefix_);
  
  private:

    void put_line (const std::string& line);
  
  } buffer;


public:

  DebugStream (const std::string& prefix);
  virtual ~DebugStream ();

  void add (std::ostream& s);
  void set_prefix (const std::string & prefix);
};

#endif

/* EOF */

//  $Id$
// 
//  TinyGetText - A small flexible gettext() replacement
//  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
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

#include <config.h>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <stdexcept>
#include <string.h>
#include <stdlib.h>
#include "iconv.hpp"

namespace TinyGetText {

IConv::IConv() : m_conv(0)
{}
 
IConv::IConv(const std::string& from_charset_, const std::string& to_charset_)
  : to_charset(to_charset_),
    from_charset(from_charset_),
    m_conv(0)
{
  // Create the converter.
  if(!(m_conv = iconv_open(to_charset.c_str(), from_charset.c_str())))
    {
      if(errno == EINVAL)
        {
          std::ostringstream sstr;
          sstr << "iconv: unsupported conversion: " << from_charset
               << " => " << to_charset << "!";
          throw std::runtime_error(sstr.str());
        }
      else
        {
          throw std::runtime_error(strerror(errno));
        }
      std::cout << "iconv: omething when very wrong" << std::endl;
      exit(1);
    }
}
 
IConv::~IConv()
{
  close();
}
 
void
IConv::close()
{
  // Free, if exists.
  if(m_conv)
    {
      iconv_close(m_conv);
      m_conv = 0;
    }
}
 
/// Convert a string from encoding to another.
std::string
IConv::convert(std::string text)
{
  if(!m_conv) return text;
 
  size_t in_size = text.size();
  size_t out_size = 4*in_size; // Worst case scenario: ASCII -> UTF-32?
  std::string result(out_size, ' ');
  ICONV_CONST char* in_str  = &text[0];
  char* out_str = &result[0];
 
  // Try to convert the text.
  if(iconv(m_conv, &in_str, &in_size, &out_str, &out_size) != 0) {
    std::cout << "TinyGetText: text: \"" << text << "\"" << std::endl;
    std::cout << "TinyGetText: Error while converting (" 
              << from_charset << " -> " << to_charset 
              << "): " << strerror(errno) << std::endl;
    exit(1);
  }
  // Eat off the spare space.
  result.resize(out_str - &result[0]);
  return result;
}

/** Convert \a which is in \a from_charset to \a to_charset and return it */
std::string
IConv::convert(const std::string& text,
               const std::string& from_charset,
               const std::string& to_charset)           
{
  if (from_charset == to_charset)
    return text;

  IConv* c = new IConv(from_charset, to_charset);
  std::string ret = c->convert(text);
  c->close();
  return ret;
}
/*
  iconv_t cd = iconv_open(to_charset.c_str(), from_charset.c_str());
  
  size_t in_len  = text.length();
  size_t out_len = text.length()*4; // Should be large enough to hold UTF-32

  char*  out_orig = new char[out_len]; // FIXME: cross fingers that this is enough
  char*  in_orig  = new char[in_len+1];
  strcpy(in_orig, text.c_str());

  char* out = out_orig;
  const char* in  = in_orig;

  //std::cout << "IN: " << (int)in << " " << in_len << " " << (int)out << " " << out_len << std::endl;
  int retval = iconv(cd, &in, &in_len, &out, &out_len);
  //std::cout << "OUT: " << (int)in << " " << in_len << " " << (int)out << " " << out_len << std::endl;

  if (retval != 0)
    {
      std::cerr << strerror(errno) << std::endl;
      std::cerr << "Error: conversion from " << from_charset
                << " to " << to_charset << " went wrong: " << retval << std::endl;
    }
  iconv_close(cd);

  
    <dolphin> your code is also buggy
<dolphin> there will be extra spaces at the end of the string
<dolphin> the lenght of the final string should be: out_str - out_orig
<dolphin> or: out_size_before_iconv_call - out_size_after_iconv_call
   
  std::string ret(out_orig, out_len);
  delete[] out_orig;
  delete[] in_orig;
  return ret;
}
*/

} // namespace TinyGetText

/* EOF */

//  $Id: generic_property_frame.hxx,v 1.1 2002/11/29 00:17:05 grumbel Exp $
// 
//  Pingus - A free Lemmings clone
//  Copyright (C) 2002 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GENERIC_PROPERTY_FRAME_HXX
#define HEADER_PINGUS_GENERIC_PROPERTY_FRAME_HXX

#include <vector>
#include <string>
#include "property_frame.hxx"

class CL_Component;

namespace EditorNS {

/** Interface for holding data pointers */
class DataBox {
public:
  /** Read the data from the data pointer */
  virtual void read_data() =0;
    
  /** Write the data from the GUI to the data pointer */
  virtual void write_data() =0;
};

class EnumDataBox;

/** PropertyFrame which can be used for simple name/value configurations */
class GenericPropertyFrame : public PropertyFrame
{
private:
  /** The title of this PropertyFrame */
  std::string title;

  /** This vector holds all the data_boxes that read and write to client data */
  std::vector<DataBox*> data_boxes;

  /** start position of the next data handler */
  int y_pos;

  /** temporary state variable for *add_enum* stuff */
  EnumDataBox* enum_data_box;
public:
  GenericPropertyFrame(const std::string& _title, CL_Component* parent);
  ~GenericPropertyFrame();

  std::string get_title () { return title; }

  /** Representation of an std::string value */
  void add_string_box(const std::string& name, std::string* value);

  /** Representation of an int value */
  void add_integer_box(const std::string& name, int* value);

  /** Representation of an float value */
  void add_float_box(const std::string& name, float* value);

  /** Representation of a boolean value */
  void add_check_box(const std::string& name, bool* value);
  
  /** Creates a listbox to represent an enumeration */
  void begin_add_enum_box(const std::string& title, int*);
  void add_enum_value(const std::string name, int value);
  void end_add_enum_box();

private:
  GenericPropertyFrame (const GenericPropertyFrame&);
  GenericPropertyFrame& operator= (const GenericPropertyFrame&);
};

} // namespace EditorNS

#endif

/* EOF */

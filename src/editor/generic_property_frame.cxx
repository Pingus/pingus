//  $Id: generic_property_frame.cxx,v 1.1 2002/11/29 00:17:05 grumbel Exp $
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

#include <iostream>
#include <ClanLib/GUI/label.h>
#include <ClanLib/GUI/inputbox.h>
#include <ClanLib/GUI/listbox.h>
#include <ClanLib/GUI/checkbox.h>
#include "../string_converter.hxx"
#include "generic_property_frame.hxx"

namespace EditorNS {

class IntegerDataBox : public DataBox
{
public:
  CL_Label*    label;
  CL_InputBox* input_box;
  int* value;

  IntegerDataBox(CL_Component* parent, int y_pos, const std::string& name, int* _value) 
    : value(_value)
  {
    label     = new CL_Label(CL_Rect(10, y_pos, 90, y_pos + 20), name, parent);
    input_box = new CL_InputBox(CL_Rect(110, y_pos, 190, y_pos + 20), to_string(*value), parent);
  }

  virtual ~IntegerDataBox()
  {
    delete label;
    delete input_box;
  }

  void read_data() 
  {
    input_box->set_text(to_string(*value));
  }
  
  void write_data() 
  {
    from_string(input_box->get_text(), *value);
  }
};

class FloatDataBox : public DataBox
{
public:
  CL_Label*    label;
  CL_InputBox* input_box;
  float* value;

  FloatDataBox(CL_Component* parent, int y_pos, const std::string& name, float* _value) 
    : value(_value)
  {
    label     = new CL_Label(CL_Rect(10, y_pos, 90, y_pos + 20), name, parent);
    input_box = new CL_InputBox(CL_Rect(110, y_pos, 190, y_pos + 20), to_string(*value), parent);
  }

  virtual ~FloatDataBox()
  {
    delete label;
    delete input_box;
  }

  void read_data() 
  {
    input_box->set_text(to_string(*value));
  }
  
  void write_data() 
  {
    from_string(input_box->get_text(), *value);
  }
};

class StringDataBox : public DataBox
{
public:
  CL_Label*    label;
  CL_InputBox* input_box;
  std::string* value;

  StringDataBox(CL_Component* parent, int y_pos, const std::string& name, std::string* _value) 
    : value(_value)
  {
    label     = new CL_Label(CL_Rect(10, y_pos, 90, y_pos + 20), name, parent);
    input_box = new CL_InputBox(CL_Rect(110, y_pos, 190, y_pos + 20), *value, parent);
  }

  virtual ~StringDataBox()
  {
    delete label;
    delete input_box;
  }

  void read_data() 
  {
    input_box->set_text(*value);
  }
  
  void write_data() 
  {
    *value = input_box->get_text();
  }
};

class BoolDataBox : public DataBox
{
public:
  CL_CheckBox* check_box;
  bool* value;

  BoolDataBox(CL_Component* parent, int y_pos, const std::string& name, bool* _value) 
    : value(_value)
  {
    check_box = new CL_CheckBox(CL_Point(10, y_pos), name, parent);
    read_data();
  }

  virtual ~BoolDataBox()
  {
    delete check_box;
  }

  void read_data() 
  {
    check_box->set_checked(*value);
  }
  
  void write_data() 
  {
    *value = check_box->is_checked();
  }
};

class EnumDataBox : public DataBox
{
public:
  CL_ListBox list_box;
  int* value;

  EnumDataBox(CL_Component* parent, int y_pos, const std::string& name, int* _value) 
    : list_box(CL_Rect(110, y_pos, 190, y_pos+20), parent), value(_value)
  {
  }

  virtual ~EnumDataBox() {}

  void read_data() 
  {
    
  }
  
  void write_data() 
  {
    
  }

  void add_item(const std::string&)
  {
  }
};

GenericPropertyFrame::GenericPropertyFrame(const std::string& _title, CL_Component* parent)
  : PropertyFrame(200, 300, parent), title(_title), y_pos(10), enum_data_box(0)
{
}

GenericPropertyFrame::~GenericPropertyFrame()
{
  for (std::vector<DataBox*>::iterator i = data_boxes.begin(); i != data_boxes.end(); ++i)
    {
      std::cout << "GenericPropertyFrame: Writing data for: " << *i << std::endl;
      (*i)->write_data();
      delete *i;
    }
}

void
GenericPropertyFrame::add_string_box(const std::string& name, std::string* value)
{
  data_boxes.push_back(new StringDataBox(this, y_pos, name, value));
  y_pos += 20;
  set_height(y_pos);
}

void
GenericPropertyFrame::add_integer_box(const std::string& name, int* value)
{
  data_boxes.push_back(new IntegerDataBox(this, y_pos, name, value));
  y_pos += 20;
  set_height(y_pos);
}

void
GenericPropertyFrame::add_float_box(const std::string& name, float* value)
{
  data_boxes.push_back(new FloatDataBox(this, y_pos, name, value));
  y_pos += 20;
  set_height(y_pos);
}

void
GenericPropertyFrame::begin_add_enum_box(const std::string& title, int*)
{
}

void
GenericPropertyFrame::add_enum_value(const std::string name, int value)
{
}

void
GenericPropertyFrame::end_add_enum_box()
{
  enum_data_box = 0;
}

void
GenericPropertyFrame::add_check_box(const std::string& name, bool* value)
{
  data_boxes.push_back(new BoolDataBox(this, y_pos, name, value));
  y_pos += 20;
  set_height(y_pos);
}

} // namespace EditorNS

/* EOF */

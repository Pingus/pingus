//  $Id: generic_property_frame.cxx,v 1.2 2002/11/30 15:06:31 grumbel Exp $
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
#include <ClanLib/GUI/radiogroup.h>
#include <ClanLib/GUI/radiobutton.h>
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
  CL_Component* parent;
  int start_y_pos;
  int y_pos;
  CL_RadioGroup radio_group;
  CL_Label      radio_label;
  typedef std::vector<std::pair<int, CL_RadioButton*> >::iterator RButtonIter;
  std::vector<std::pair<int, CL_RadioButton*> > radio_buttons;

  /** Pointer to the value that should be changed */
  int* value;

  EnumDataBox(CL_Component* p, int y, const std::string& name, int* _value) 
    : parent(p), 
      start_y_pos(y),
      y_pos(y),
      radio_label(CL_Rect(10, y_pos, 90, y_pos+20), name, parent),
      value(_value)
  {
  }

  virtual ~EnumDataBox() {}

  void read_data() 
  {
    for (RButtonIter i = radio_buttons.begin(); i != radio_buttons.end(); ++i)
      {
        if (i->first == *value)
          {
            i->second->set_checked(true);
            return;
          }
      }
  }
  
  void write_data() 
  {
    for (RButtonIter i = radio_buttons.begin(); i != radio_buttons.end(); ++i)
      {
        if (i->second->is_checked())
          {
            *value = i->first;
            return;
          }
      }    
  }

  void add_item(const std::string& name, int item_value)
  {
    CL_RadioButton* rbutton = new CL_RadioButton(CL_Point(110, y_pos), name, parent);
    radio_buttons.push_back(std::pair<int, CL_RadioButton*>(item_value, rbutton));
    radio_group.add(rbutton);

    std::cout << "Value: " << *value << " ItemValue: " << item_value << std::endl;

    if (item_value == *value)
      rbutton->set_checked(true);
    else
      rbutton->set_checked(false);

    y_pos += 20;
  }
  
  int get_height() {
    return y_pos - start_y_pos + 5;
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
  set_height(y_pos + 5);
}

void
GenericPropertyFrame::add_integer_box(const std::string& name, int* value)
{
  data_boxes.push_back(new IntegerDataBox(this, y_pos, name, value));
  y_pos += 20;
  set_height(y_pos + 5);
}

void
GenericPropertyFrame::add_float_box(const std::string& name, float* value)
{
  data_boxes.push_back(new FloatDataBox(this, y_pos, name, value));
  y_pos += 20;
  set_height(y_pos + 5);
}

void
GenericPropertyFrame::begin_add_enum_box(const std::string& title, int* value)
{
  assert(enum_data_box == 0);
  enum_data_box = new EnumDataBox(this, y_pos, title, value);
}

void
GenericPropertyFrame::add_enum_value(const std::string& name, int value)
{
  assert(enum_data_box);
  enum_data_box->add_item(name, value);
}

void
GenericPropertyFrame::end_add_enum_box()
{
  assert(enum_data_box);

  data_boxes.push_back(enum_data_box);
  y_pos += enum_data_box->get_height();
  set_height(y_pos + 5);

  enum_data_box = 0;
}

void
GenericPropertyFrame::add_check_box(const std::string& name, bool* value)
{
  data_boxes.push_back(new BoolDataBox(this, y_pos, name, value));
  y_pos += 20;
  set_height(y_pos + 5);
}

} // namespace EditorNS

/* EOF */

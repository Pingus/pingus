//  $Id: combobox.hxx,v 1.16 2005/11/10 21:37:06 Jave27 Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 2005 Ingo Ruhnke <grumbel@gmx.de>
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

#ifndef HEADER_PINGUS_GUI_COMBOBOX_HXX
#define HEADER_PINGUS_GUI_COMBOBOX_HXX

#include <vector>
#include <string>
#include "component.hxx"
#include "../vector.hxx"

namespace Pingus {

namespace GUI {

/** A ComboItems holds a specific item in a Combobox (string, id, etc.) */
class ComboItem
{
protected:
	/* The item's hidden ID field */
	int					id;

	/** string that displays when printed on the Combobox */
	std::string str;

	/** Should this item be deleted when the Combobox dies? */
	bool delete_item;

public:
	/** Constructors */
	ComboItem () { delete_item = true; }
	ComboItem (int i, std::string s, bool d = true) 
	{
		id = i;
		str =s;
		delete_item = d;
	}

	virtual ~ComboItem () { }

	/** Returns this item's ID */
	int get_id() { return id; }

	/** Returns this item's string */
	std::string get_string() { return str; }

	/** Returns the value of del_item */
	bool delete_it() { return delete_item; }

	/** Set this item's ID */
	void set_id(int i) { id = i; }

	/** Set this item's string */
	void set_string(std::string s) { str = s; }

private:
  ComboItem (const ComboItem&);
  ComboItem& operator= (const ComboItem&);
};	// ComboItem class



/** The Combobox class gives you a drop-down list of items to choose from. */
class Combobox : public Component
{
protected:
	/** List of items in this Combobox */
	std::vector<ComboItem*> item_list;

	/** The currently selected item in the list */
	ComboItem* current_item;

	/** Whether or not the list is showing right now */
	bool drop_down;

	/** Is the mouse hovering over this object? */
	bool hover;

	/** Where the mouse is located */
	Vector mouse_pos;

	/** Width of the Combobox */
	float width;

	/** Height of EACH ITEM in the Combobox.  So, when drop_down is false, this
		should be the height of the entire widget/component.  When drop_down is 
		true, the entire widget's height should be this height times the number of
		items */
	float height;

	/** Location of the Combobox */
	Vector pos;

	/** Whether or not this combobox is enabled */
	bool enabled;
	
	/** Label that prints to the left of the drop-down */
	std::string label;

public:
	/** Constructor */
	Combobox (Vector p, std::string label = "");
	
	/** Destructor */
	virtual ~Combobox ();

	/** Add an item to the list. 
		@param del_item Set to true if you want this Combobox to delete the 
			ComboItem when it's destroyed */
	virtual void add(ComboItem* item);

	/** Remove an item from the list */
	virtual void remove(ComboItem* item);

	/** Clears all items from combobox */
	virtual void clear();

	/** Return a pointer to the selected item.  Returns 0 if nothing is selected */
	ComboItem* get_selected_item() { return current_item; }

	/** Sets the selected item to the given ComboItem.  Returns false if that item
		doesn't exist */
	bool set_selected_item(ComboItem*);

	/** Draw this Combobox and all of it's items if selected */
  virtual void draw (DrawingContext& gc);

	/** Tells the gui_manager if the mouse is on top of the Combobox */
  virtual bool is_at (int x, int y);

	/** Returns the height of the Combobox at this given moment.  Will be tall if 
		drop_down = true, and short if drop_down = false */
	virtual float get_height();

	/** Returns the width of the Combobox */
	virtual float get_width();

  /** Gets emmited when a button is pressed and released over the
      same component */
  virtual void on_primary_button_click (int x, int y);
	
	virtual void on_pointer_enter() { hover = true; }
	virtual void on_pointer_leave() { hover = false; }
	virtual void on_pointer_move(int x, int y) { mouse_pos = Vector(x, y); }
	
	/** Sets whether or not this combobox is clickable */
	virtual void set_enabled(bool e) { enabled = e; }
	
	/** Returns whether or not the combobox is enabled */
	virtual bool is_enabled() { return enabled; }
	
	/** Sets the label */
	virtual void set_label(std::string l) { label = l; }

private:
	Combobox();
  Combobox (const Combobox&);
  Combobox& operator= (const Combobox&);

};	// Combobox class

}		// GUI namespace
}		// Pingus namespace

#endif

/* EOF */

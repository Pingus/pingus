//  $Id: panel_icons.hxx,v 1.10 2003/10/18 23:17:27 grumbel Exp $
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

#ifndef HEADER_PINGUS_EDITOR_PANEL_ICONS_HXX
#define HEADER_PINGUS_EDITOR_PANEL_ICONS_HXX

#include "panel.hxx"

namespace Pingus {
namespace EditorNS {

class PanelIconNew : public PanelIcon
{
public:
  PanelIconNew();
  virtual void on_click();

private:
  PanelIconNew (const PanelIconNew&);
  PanelIconNew& operator= (const PanelIconNew&);
};

class PanelIconLoad : public PanelIcon
{
public:
  PanelIconLoad();
  virtual void on_click();

private:
  PanelIconLoad (const PanelIconLoad&);
  PanelIconLoad& operator= (const PanelIconLoad&);
};

class PanelIconSave : public PanelIcon
{
public:
  PanelIconSave();
  virtual void on_click();

private:
  PanelIconSave (const PanelIconSave&);
  PanelIconSave& operator= (const PanelIconSave&);
};

class PanelIconCopy : public PanelIcon
{
public:
  PanelIconCopy();
  virtual void on_click();

private:
  PanelIconCopy (const PanelIconCopy&);
  PanelIconCopy& operator= (const PanelIconCopy&);
};

class PanelIconPaste : public PanelIcon
{
public:
  PanelIconPaste();
  virtual void on_click();

private:
  PanelIconPaste (const PanelIconPaste&);
  PanelIconPaste& operator= (const PanelIconPaste&);
};

class PanelIconLevelProperties : public PanelIcon
{
public:
  PanelIconLevelProperties();
  virtual void on_click();

private:
  PanelIconLevelProperties (const PanelIconLevelProperties&);
  PanelIconLevelProperties& operator= (const PanelIconLevelProperties&);
};

class PanelIconObjectProperties : public PanelIcon
{
public:
  PanelIconObjectProperties();
  virtual void on_click();

private:
  PanelIconObjectProperties (const PanelIconObjectProperties&);
  PanelIconObjectProperties& operator= (const PanelIconObjectProperties&);
};

class PanelIconDelete : public PanelIcon
{
public:
  PanelIconDelete();
  virtual void on_click();

private:
  PanelIconDelete (const PanelIconDelete&);
  PanelIconDelete& operator= (const PanelIconDelete&);
};

class PanelIconExit : public PanelIcon
{
public:
  PanelIconExit();
  virtual void on_click();

private:
  PanelIconExit (const PanelIconExit&);
  PanelIconExit& operator= (const PanelIconExit&);
};

class PanelIconRun : public PanelIcon
{
public:
  PanelIconRun();
  virtual void on_click();

private:
  PanelIconRun (const PanelIconRun&);
  PanelIconRun& operator= (const PanelIconRun&);
};

class PanelIconInsertObject : public PanelIcon
{
public:
  PanelIconInsertObject();
  virtual void on_click();

private:
  PanelIconInsertObject (const PanelIconInsertObject&);
  PanelIconInsertObject& operator= (const PanelIconInsertObject&);
};

class PanelIconZoomOut : public PanelIcon
{
public:
  PanelIconZoomOut();
  virtual void on_click();

private:
  PanelIconZoomOut (const PanelIconZoomOut&);
  PanelIconZoomOut& operator= (const PanelIconZoomOut&);
};

class PanelIconZoomIn : public PanelIcon
{
public:
  PanelIconZoomIn();
  virtual void on_click();

private:
  PanelIconZoomIn (const PanelIconZoomIn&);
  PanelIconZoomIn& operator= (const PanelIconZoomIn&);
};

class PanelIconRegionZoom : public PanelIcon
{
public:
  PanelIconRegionZoom();
  virtual void on_click();

private:
  PanelIconRegionZoom (const PanelIconRegionZoom&);
  PanelIconRegionZoom& operator= (const PanelIconRegionZoom&);
};

class PanelIconSetupActions : public PanelIcon
{
public:
  PanelIconSetupActions ();
  virtual void on_click();

private:
  PanelIconSetupActions (const PanelIconSetupActions&);
  PanelIconSetupActions& operator= (const PanelIconSetupActions&);
};


class PanelIconHelp : public PanelIcon
{
public:
  PanelIconHelp ();
  virtual void on_click();

private:
  PanelIconHelp (const PanelIconHelp&);
  PanelIconHelp& operator= (const PanelIconHelp&);
};

} // namespace EditorNS
} // namespace Pingus

#endif

/* EOF */

#  $Id: Makefile.am,v 1.18 2003/04/12 13:34:57 grumbel Exp $
#
#  Pingus - A free Lemmings clone
#  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

## Override automake so that "make install" puts these in proper place:
pkgdatadir = $(datadir)/games/@PACKAGE@/levels

nobase_pkgdata_DATA = \
	$(wildcard tutorial/*.xml) \
        $(wildcard playable/*.xml) \
        $(wildcard wip/*.xml)      \
        $(wildcard test/*.xml)

EXTRA_DIST = README  \
             $(nobase_pkgdata_DATA)

# EOF #

#  $Id: Makefile.am,v 1.25 2001/05/15 17:59:46 grumbel Exp $
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

pkgdatadir = $(datadir)/games/@PACKAGE@/images

EXTRA_DIST = $(shell ./extract_images.sh)

my_pkg_data = $(EXTRA_DIST)

# We don't use automake's stuff to install the data, since it is
# unable to handle recursive directory trees.
# The list was produced by:
# find . -type d -and -not -name "CVS" -and -not -name ".xvpics"  | sed "s/^\./\$(pkgdatadir)/" | sed "s/\$/ \\\\/"
install-data-local: $(my_pkg_data)
	$(mkinstalldirs) \
$(pkgdatadir) \
$(pkgdatadir)/banners \
$(pkgdatadir)/backgrounds \
$(pkgdatadir)/entrances \
$(pkgdatadir)/entrances/sortie \
$(pkgdatadir)/exits \
$(pkgdatadir)/fonts \
$(pkgdatadir)/groundpieces \
$(pkgdatadir)/groundpieces/bridge \
$(pkgdatadir)/groundpieces/bridge/space \
$(pkgdatadir)/groundpieces/bridge/misc \
$(pkgdatadir)/groundpieces/misc \
$(pkgdatadir)/groundpieces/sweets \
$(pkgdatadir)/groundpieces/solid \
$(pkgdatadir)/groundpieces/solid/rock \
$(pkgdatadir)/groundpieces/solid/crystal \
$(pkgdatadir)/groundpieces/solid/misc \
$(pkgdatadir)/groundpieces/transparent \
$(pkgdatadir)/groundpieces/transparent/misc \
$(pkgdatadir)/groundpieces/transparent/foliage \
$(pkgdatadir)/groundpieces/transparent/industrial \
$(pkgdatadir)/groundpieces/ground \
$(pkgdatadir)/groundpieces/ground/test \
$(pkgdatadir)/groundpieces/ground/mushroom \
$(pkgdatadir)/groundpieces/ground/desert \
$(pkgdatadir)/groundpieces/ground/snow \
$(pkgdatadir)/groundpieces/ground/jungle \
$(pkgdatadir)/groundpieces/ground/industrial \
$(pkgdatadir)/groundpieces/ground/misc \
$(pkgdatadir)/groundpieces/ground/crystal \
$(pkgdatadir)/groundpieces/ground/foliage \
$(pkgdatadir)/groundpieces/ground/rock \
$(pkgdatadir)/groundpieces/ground/sortie \
$(pkgdatadir)/groundpieces/ground/space \
$(pkgdatadir)/groundpieces/ground/sweets \
$(pkgdatadir)/groundpieces/ground/signposts \
$(pkgdatadir)/hotspots \
$(pkgdatadir)/hotspots/misc \
$(pkgdatadir)/hotspots/space \
$(pkgdatadir)/hotspots/signposts \
$(pkgdatadir)/liquids \
$(pkgdatadir)/menu \
$(pkgdatadir)/particles \
$(pkgdatadir)/textures \
$(pkgdatadir)/textures/.axvpixs \
$(pkgdatadir)/themes \
$(pkgdatadir)/traps \
$(pkgdatadir)/buttons \
$(pkgdatadir)/story \
$(pkgdatadir)/misc \
$(pkgdatadir)/worldmaps \
$(pkgdatadir)/worldobjs \
$(pkgdatadir)/core \
$(pkgdatadir)/core/cursors \
$(pkgdatadir)/core/editor \
$(pkgdatadir)/core/buttons \
$(pkgdatadir)/core/menu \
$(pkgdatadir)/core/misc \
$(pkgdatadir)/core/worldmap \
$(pkgdatadir)/core/pingus \
$(pkgdatadir)/core/gui \
$(pkgdatadir)/core/gui/gui \
$(pkgdatadir)/core/gui/button \
$(pkgdatadir)/core/gui/checkbox \
$(pkgdatadir)/core/gui/combobox \
$(pkgdatadir)/core/gui/listbox \
$(pkgdatadir)/core/gui/menu \
$(pkgdatadir)/core/gui/scrollbar \
$(pkgdatadir)/cursors \
$(pkgdatadir)/pingus
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(pkgdatadir)
	@list='$(my_pkg_data)'; for p in $$list; do \
	  if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(pkgdatadir)/$$p"; \
	    $(INSTALL_DATA) $(srcdir)/$$p $(DESTDIR)$(pkgdatadir)/$$p; \
	  else if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(pkgdatadir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(pkgdatadir)/$$p; \
	  fi; fi; \
	done

# cat ../data/*.scr | sed -n "s/^[ 	]*[A-Za-z0-9_]*[ 	]*=[ 	]*\([^ ]*\) (.*);/\1/p"

## EOF ##
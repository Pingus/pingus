# Pingus - A free Lemmings clone
# Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

EXTRA_DIST = $(wildcard *.po)
pkgdata_DATA = $(EXTRA_DIST)

# make[1]: Entering directory `/tmp/trunk/po'
# /usr/bin/xgettext --default-domain=pingus --directory=.. \
#   --add-comments=TRANSLATORS: --keyword=_ --keyword=N_ \
#   --files-from=./POTFILES.in \
#   --copyright-holder='Ingo Ruhnke'
# test ! -f pingus.po || { \
#   if test -f ./pingus.pot; then \
#     sed -f remove-potcdate.sed < ./pingus.pot > pingus.1po && \
#     sed -f remove-potcdate.sed < pingus.po > pingus.2po && \
#     if cmp pingus.1po pingus.2po >/dev/null 2>&1; then \
#       rm -f pingus.1po pingus.2po pingus.po; \
#     else \
#       rm -f pingus.1po pingus.2po ./pingus.pot && \
#       mv pingus.po ./pingus.pot; \
#     fi; \
#   else \
#     mv pingus.po ./pingus.pot; \
#   fi; \
# }
# make[1]: Leaving directory `/tmp/trunk/po'
# /usr/bin/msgmerge --update cs.po pingus.pot
# ................... done.
# rm -f cs.gmo && /usr/bin/msgfmt -c --statistics -o cs.gmo cs.po
# 176 translated messages, 6 fuzzy translations.
# /usr/bin/msgmerge --update de.po pingus.pot
# ................... done.


update-po:
	xgettext --default-domain=pingus --directory=../.. \
   --add-comments=TRANSLATORS: --keyword=_ --keyword=N_ \
   --files-from=./POTFILES --output=pingus.pot \
   --copyright-holder='Ingo Ruhnke'; \
   for lang in *.po; do \
     echo "Updating $$lang: "; \
     msgmerge --update $$lang pingus.pot; \
   done

# EOF #

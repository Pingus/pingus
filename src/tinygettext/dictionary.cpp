//  $Id$
// 
//  TinyGetText - A small flexible gettext() replacement
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
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

#include "dictionary.hpp"

namespace TinyGetText {

Dictionary::Dictionary(const LanguageDef& language_, const std::string& charset_)
  : language(language_), charset(charset_)
{
}

Dictionary::Dictionary()
  : language(lang_en)
{
}

std::string
Dictionary::get_charset() const
{
  return charset;
}

void
Dictionary::set_charset(const std::string& charset_)
{
  charset = charset_;
}

void
Dictionary::set_language(const LanguageDef& lang)
{
  language = lang;
}

std::string
Dictionary::translate(const std::string& msgid, const std::string& msgid2, int num) 
{
  PluralEntries::iterator i = plural_entries.find(msgid);
  std::map<int, std::string>& msgstrs = i->second;

  if (i != plural_entries.end() && !msgstrs.empty())
    {
      int g = language.plural(num);
      std::map<int, std::string>::iterator j = msgstrs.find(g);
      if (j != msgstrs.end())
        {
          return j->second;
        }
      else
        {
          // Return the first translation, in case we can't translate the specific number
          return msgstrs.begin()->second;
        }
    }
  else
    {
#ifdef TRANSLATION_DEBUG
      std::cerr << "Warning: Couldn't translate: " << msgid << std::endl;
      std::cerr << "Candidates: " << std::endl;
      for (PluralEntries::iterator i = plural_entries.begin(); i != plural_entries.end(); ++i)
        std::cout << "'" << i->first << "'" << std::endl;
#endif

      if (plural2_1(num)) // default to english rules
        return msgid2;
      else
        return msgid;
    }
}

std::string
Dictionary::translate(const std::string& msgid) 
{
  Entries::iterator i = entries.find(msgid);
  if (i != entries.end() && !i->second.empty())
    {
      return i->second;
    }
  else
    {
#ifdef TRANSLATION_DBEUG
      std::cout << "Error: Couldn't translate: " << msgid << std::endl;
#endif
      return msgid;
    }
}
  
void
Dictionary::add_translation(const std::string& msgid, const std::string& ,
                            const std::map<int, std::string>& msgstrs)
{
  // Do we need msgid2 for anything? its after all supplied to the
  // translate call, so we just throw it away
  plural_entries[msgid] = msgstrs;
}

void 
Dictionary::add_translation(const std::string& msgid, const std::string& msgstr) 
{
  entries[msgid] = msgstr;
}

} // namespace TinyGetText

/* EOF */

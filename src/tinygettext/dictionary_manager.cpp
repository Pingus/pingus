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

#include <iostream>
#include <sys/types.h>
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#include "../win32/uce_dirent.h"
#endif

#include <fstream>
#include "dictionary_manager.hpp"
#include "po_file_reader.hpp"

namespace TinyGetText {

static bool has_suffix(const std::string& lhs, const std::string rhs)
{
  if (lhs.length() < rhs.length())
    return false;
  else
    return lhs.compare(lhs.length() - rhs.length(), rhs.length(), rhs) == 0;
}

DictionaryManager::DictionaryManager()
  : empty_dict(lang_en, "ISO-8859-1"),
    current_dict(&empty_dict)
{
  parseLocaleAliases();
  // setup language from environment vars
  const char* lang = getenv("LC_ALL");
  if(!lang)
    lang = getenv("LC_MESSAGES");
  if(!lang)
    lang = getenv("LANG");
  
  if(lang)
    set_language(lang);
}

void
DictionaryManager::parseLocaleAliases()
{
  // try to parse language alias list
  std::ifstream in("/usr/share/locale/locale.alias");
  
  char c = ' ';
  while(in.good() && !in.eof()) {
    while(isspace(c) && !in.eof())
      in.get(c);
    
    if(c == '#') { // skip comments
      while(c != '\n' && !in.eof())
        in.get(c);
      continue;
    }
    
    std::string alias;
    while(!isspace(c) && !in.eof()) {
      alias += c;
      in.get(c);
    }
    while(isspace(c) && !in.eof())
      in.get(c);
    std::string language;
    while(!isspace(c) && !in.eof()) {
      language += c;
      in.get(c);
    }

    if(in.eof())
      break;
    set_language_alias(alias, language);
  }
}
  
Dictionary&
DictionaryManager::get_dictionary(const std::string& spec)
{
  std::string lang = get_language_from_spec(spec);
  Dictionaries::iterator i = dictionaries.find(get_language_from_spec(lang));
  if (i != dictionaries.end())
    {
      return i->second;
    }
  else // Dictionary for languages lang isn't loaded, so we load it
    {
      Dictionary& dict = dictionaries[lang];

      dict.set_language(get_language_def(lang));

      for (SearchPath::iterator p = search_path.begin(); p != search_path.end(); ++p)
        {
          DIR* dir = opendir(p->c_str());
          if (!dir)
            {
              std::cerr << "TinyGetText: Error: opendir() failed on " << *p << std::endl;
            }
          else
            {
              struct dirent* ent;
              while((ent = readdir(dir)))
                {
                  if (std::string(ent->d_name) == lang + ".po")
                    {
                      std::string pofile = *p + "/" + ent->d_name;
                      std::ifstream in(pofile.c_str());
                      if (!in)
                        {
                          std::cerr << "Error: Failure file opening: " << pofile << std::endl;
                        }
                      else
                        {
                          POFileReader reader(in, dict);
                        }
                    }
                }
              closedir(dir);
            }
        }

      return dict;
    }
}

std::set<std::string>
DictionaryManager::get_languages()
{
  std::set<std::string> languages;

  for (SearchPath::iterator p = search_path.begin(); p != search_path.end(); ++p)
    {
      DIR* dir = opendir(p->c_str());
      if (!dir)
        {
          std::cerr << "Error: opendir() failed on " << *p << std::endl;
        }
      else
        {
          struct dirent* ent;
          while((ent = readdir(dir)))
            {
              if (has_suffix(ent->d_name, ".po"))
                {
                  std::string filename = ent->d_name;
                  languages.insert(filename.substr(0, filename.length()-3));
                }
            }
          closedir(dir);
        }
    }  
  return languages;
}

void
DictionaryManager::set_language(const std::string& lang)
{
  language = get_language_from_spec(lang);
  current_dict = & (get_dictionary(language));
}

void
DictionaryManager::set_language_alias(const std::string& alias,
    const std::string& language)
{
  language_aliases.insert(std::make_pair(alias, language));
}

std::string
DictionaryManager::get_language_from_spec(const std::string& spec)
{
  std::string lang = spec;
  Aliases::iterator i = language_aliases.find(lang);
  if(i != language_aliases.end()) {
    lang = i->second;
  }
  
  std::string::size_type s = lang.find_first_of("_.");
  if(s == std::string::npos)
    return lang;

  return std::string(lang, 0, s);  
}

void
DictionaryManager::add_directory(const std::string& pathname)
{
  dictionaries.clear(); // adding directories invalidates cache
  search_path.push_back(pathname);
  set_language(language);
}

} // namespace TinyGetText

/* EOF */

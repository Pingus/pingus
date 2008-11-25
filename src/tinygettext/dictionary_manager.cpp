//  $Id: dictionary_manager.cpp 3353 2007-10-28 08:05:10Z grumbel $
// 
//  TinyGetText - A small flexible gettext() replacement
//  Copyright (C) 2004 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#ifndef WIN32
#  include <dirent.h>
#else
# include "../win32/uce_dirent.h"
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
  : empty_dict(&lang_en, "ISO-8859-1"),
    language(&lang_en),
    current_dict(&empty_dict)    
{
#ifndef WIN32
  parseLocaleAliases();
#endif

  // setup language from environment vars
  const char* lang = getenv("LC_ALL");
  if (!lang)
    lang = getenv("LC_MESSAGES");
  if (!lang)
    lang = getenv("LANG");
  
  if (lang)
    set_current_dictionary(lang);
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
    std::string lang;
    while(!isspace(c) && !in.eof()) {
      lang += c;
      in.get(c);
    }

    if(in.eof())
      break;
    
    set_language_alias(alias, lang);
  }
}
  
Dictionary&
DictionaryManager::get_dictionary(LanguageDef* lang)
{
  Dictionaries::iterator i = dictionaries.find(lang);
  if (i != dictionaries.end())
    {
      return i->second;
    }
  else // Dictionary for languages lang isn't loaded, so we load it
    {
      Dictionary& dict = dictionaries[lang];

      dict.set_language(lang);

      for(SearchPath::iterator p = search_path.begin(); p != search_path.end(); ++p)
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
                  if (std::string(ent->d_name) == std::string(lang->code) + ".po")
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
  languages.insert("en");

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

std::string
DictionaryManager::get_current_language()
{
  return current_dict->get_language()->name;
}

void
DictionaryManager::set_current_dictionary(const std::string& lang)
{
  LanguageDef* new_lang = get_canonical_language(lang);
  if (!new_lang)
    {
      std::cout << "Error: DictionaryManager: Couldn't find LanguageDef for '" << lang
                << "', leaving current dictionary unchanged" << std::endl;
    }
  else
    {
      language = new_lang;
      // std::cout << "Language: supplied: '" << lang << "' -> canonical: '" << language->code << "'" << std::endl;
      current_dict = &(get_dictionary(language));
    }
}

void
DictionaryManager::set_language_alias(const std::string& alias,
    const std::string& language)
{
  language_aliases.insert(std::make_pair(alias, language));
}

LanguageDef*
DictionaryManager::get_canonical_language(const std::string& spec)
{
  std::string lang_code = spec;

  // Check for language aliases
  Aliases::iterator i = language_aliases.find(spec);
  if(i != language_aliases.end()) 
    lang_code = i->second;

  // Cut away any specification of the codeset (i.e. de_DE.ISO-8859-1)
  std::string::size_type s = lang_code.find_first_of(".");

  if (s != std::string::npos)
    lang_code = std::string(spec, 0, s);

  LanguageDef* lang_def = get_language_def(lang_code);
  if (lang_def)
    return lang_def;
  else if (lang_code.length() == 5) // 'en_EN' failed, try 'en'
    return get_language_def(lang_code.substr(0, 2));
  else 
    return 0;
}

void
DictionaryManager::add_directory(const std::string& pathname)
{
  dictionaries.clear(); // adding directories invalidates cache
  search_path.push_back(pathname);
  current_dict = &(get_dictionary(language));
}

} // namespace TinyGetText

/* EOF */

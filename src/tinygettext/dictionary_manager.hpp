//  $Id: dictionary_manager.hpp 3333 2007-10-27 04:52:21Z grumbel $
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

#ifndef HEADER_DICTIONARY_MANAGER_HPP
#define HEADER_DICTIONARY_MANAGER_HPP

#include <vector>
#include <set>
#include "dictionary.hpp"

namespace TinyGetText {

/** Manager class for dictionaries, you give it a bunch of directories
    with .po files and it will then automatically load the right file
    on demand depending on which language was set. */
class DictionaryManager
{
private:
  Dictionary empty_dict;
  
  typedef std::map<LanguageDef*, Dictionary> Dictionaries;
  Dictionaries dictionaries;
  typedef std::vector<std::string> SearchPath;
  SearchPath search_path;
  typedef std::map<std::string, std::string> Aliases;
  Aliases language_aliases;
  // The two (de) or five letter (de_DE) language code
  LanguageDef* language;
  Dictionary*  current_dict;
  
public:
  DictionaryManager();

  /** Return the currently active dictionary, if none is set, an empty
      dictionary is returned. */
  Dictionary& get_dictionary()
  { return *current_dict; }

  /** Get dictionary for the given lang_code */
  Dictionary& get_dictionary(LanguageDef* lang);

  /** Set a language based on a four? letter country code */
  void set_current_dictionary(const std::string& langspec);

  /** Define an alias for a language */
  void set_language_alias(const std::string& alias, const std::string& lang);

  /** Add a directory to the search path for dictionaries */
  void add_directory(const std::string& pathname);
  
  /** Return a set of the available languages in their country code */
  std::set<std::string> get_languages();

private:
  void parseLocaleAliases();

  // de_DE.ISO-8859-1 -> de_DE, german -> de_DE, etc.
  LanguageDef* get_canonical_language(const std::string& language);
};

} // namespace TinyGetText

#endif

/* EOF */

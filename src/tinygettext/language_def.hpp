//  $Id: language_def.hpp 3333 2007-10-27 04:52:21Z grumbel $
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

#ifndef HEADER_LANGUAGE_DEF_HPP
#define HEADER_LANGUAGE_DEF_HPP

#include <string>

namespace TinyGetText {

typedef int (*PluralFunc)(int n);

struct LanguageDef {
  const char* code;
  const char* name;
  int         nplural;
  PluralFunc  plural;

  LanguageDef(const char* code_, const char* name_,  int nplural_, PluralFunc plural_)
    : code(code_), name(name_), nplural(nplural_), plural(plural_)
  {}
};

/** Read the content of the .po file given as \a in into the
    dictionary given as \a dict */
LanguageDef* get_language_def(const std::string& name);

extern LanguageDef lang_hu;
extern LanguageDef lang_ja;
extern LanguageDef lang_ko;
extern LanguageDef lang_tr;
extern LanguageDef lang_da;
extern LanguageDef lang_nl;
extern LanguageDef lang_en;
extern LanguageDef lang_fo;
extern LanguageDef lang_de;
extern LanguageDef lang_nb;
extern LanguageDef lang_no;
extern LanguageDef lang_nn;
extern LanguageDef lang_sv;
extern LanguageDef lang_et;
extern LanguageDef lang_fi;
extern LanguageDef lang_el;
extern LanguageDef lang_he;
extern LanguageDef lang_it;
extern LanguageDef lang_pt;
extern LanguageDef lang_es;
extern LanguageDef lang_eo;
extern LanguageDef lang_fr;
extern LanguageDef lang_pt_BR;
extern LanguageDef lang_lv;
extern LanguageDef lang_ga;
extern LanguageDef lang_lt;
extern LanguageDef lang_hr;
extern LanguageDef lang_cs;
extern LanguageDef lang_ru;
extern LanguageDef lang_uk;
extern LanguageDef lang_sk;
extern LanguageDef lang_pl;
extern LanguageDef lang_sl;

int plural1(int );
int plural2_1(int n);
int plural2_2(int n);
int plural3_lv(int n);
int plural3_ga(int n);
int plural3_lt(int n);
int plural3_1(int n);
int plural3_sk(int n);
int plural3_pl(int n);
int plural3_sl(int n);

} // namespace TinyGetText


#endif

/* EOF */

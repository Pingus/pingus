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

#include "language_def.hpp"

namespace TinyGetText {

int plural1(int )     { return 0; }
int plural2_1(int n)  { return (n != 1); }
int plural2_2(int n)  { return (n > 1); }
int plural3_lv(int n) { return (n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2); }
int plural3_ga(int n) { return n==1 ? 0 : n==2 ? 1 : 2; }
int plural3_lt(int n) { return (n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2); }
int plural3_1(int n)  { return (n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2); }
int plural3_sk(int n) { return (n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2; }
int plural3_pl(int n) { return (n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2); }
int plural3_sl(int n) { return (n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3); }

/** Language Definitions */
//*{
LanguageDef lang_hu("hu", "Hungarian",         1, plural1); // "nplurals=1; plural=0;"
LanguageDef lang_ja("ja", "Japanese",          1, plural1); // "nplurals=1; plural=0;"
LanguageDef lang_ko("ko", "Korean",            1, plural1); // "nplurals=1; plural=0;"
LanguageDef lang_tr("tr", "Turkish",           1, plural1); // "nplurals=1; plural=0;"
LanguageDef lang_da("da", "Danish",            2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_nl("nl", "Dutch",             2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_en("en", "English",           2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_fo("fo", "Faroese",           2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_de("de", "German",            2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_nb("nb", "Norwegian Bokmal",  2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_no("no", "Norwegian",         2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_nn("nn", "Norwegian Nynorsk", 2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_sv("sv", "Swedish",           2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_et("et", "Estonian",          2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_fi("fi", "Finnish",           2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_el("el", "Greek",             2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_he("he", "Hebrew",            2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_it("it", "Italian",           2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_pt("pt", "Portuguese",        2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_es("es", "Spanish",           2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_eo("eo", "Esperanto",         2, plural2_1); // "nplurals=2; plural=(n != 1);"
LanguageDef lang_fr("fr", "French",            2, plural2_2); // "nplurals=2; plural=(n > 1);"
LanguageDef lang_pt_BR("pt_BR", "Brazilian",   2, plural2_2); // "nplurals=2; plural=(n > 1);"
LanguageDef lang_lv("lv", "Latvian",           3, plural3_lv); // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n != 0 ? 1 : 2);"
LanguageDef lang_ga("ga", "Irish",             3, plural3_ga); // "nplurals=3; plural=n==1 ? 0 : n==2 ? 1 : 2;"
LanguageDef lang_lt("lt", "Lithuanian",        3, plural3_lt); // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageDef lang_hr("hr", "Croatian",          3, plural3_1); // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageDef lang_cs("cs", "Czech",             3, plural3_1); // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageDef lang_ru("ru", "Russian",           3, plural3_1); // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageDef lang_uk("uk", "Ukrainian",         3, plural3_1); // "nplurals=3; plural=(n%10==1 && n%100!=11 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);"
LanguageDef lang_sk("sk", "Slovak",            3, plural3_sk); // "nplurals=3; plural=(n==1) ? 0 : (n>=2 && n<=4) ? 1 : 2;"
LanguageDef lang_pl("pl", "Polish",            3, plural3_pl); // "nplurals=3; plural=(n==1 ? 0 : n%10>=2 && n%10<=4 && (n%100<10 || n%100>=20) ? 1 : 2);
LanguageDef lang_sl("sl", "Slovenian",         3, plural3_sl); // "nplurals=4; plural=(n%100==1 ? 0 : n%100==2 ? 1 : n%100==3 || n%100==4 ? 2 : 3);"
//*}

LanguageDef&
get_language_def(const std::string& name)
{
  if (name == "hu") return lang_hu;
  else if (name == "ja") return lang_ja;
  else if (name == "ko") return lang_ko;
  else if (name == "tr") return lang_tr;
  else if (name == "da") return lang_da;
  else if (name == "nl") return lang_nl;
  else if (name == "en") return lang_en;
  else if (name == "fo") return lang_fo;
  else if (name == "de") return lang_de;
  else if (name == "nb") return lang_nb;
  else if (name == "no") return lang_no;
  else if (name == "nn") return lang_nn;
  else if (name == "sv") return lang_sv;
  else if (name == "et") return lang_et;
  else if (name == "fi") return lang_fi;
  else if (name == "el") return lang_el;
  else if (name == "he") return lang_he;
  else if (name == "it") return lang_it;
  else if (name == "pt") return lang_pt;
  else if (name == "es") return lang_es;
  else if (name == "eo") return lang_eo;
  else if (name == "fr") return lang_fr;
  else if (name == "pt_BR") return lang_pt_BR;
  else if (name == "lv") return lang_lv;
  else if (name == "ga") return lang_ga;
  else if (name == "lt") return lang_lt;
  else if (name == "hr") return lang_hr;
  else if (name == "cs") return lang_cs;
  else if (name == "ru") return lang_ru;
  else if (name == "uk") return lang_uk;
  else if (name == "sk") return lang_sk;
  else if (name == "pl") return lang_pl;
  else if (name == "sl") return lang_sl;
  else return lang_en; 
}

} // namespace TinyGetText 

/* EOF */

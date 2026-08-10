// tinygettext - A gettext replacement that works directly on .po files
// Copyright (c) 2006 Ingo Ruhnke <grumbel@gmail.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgement in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include "tinygettext/language.hpp"

#include <assert.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace tinygettext {

struct LanguageSpec {
  /** Language code: "de", "en", ... */
  const char* language;

  /** Country code: "BR", "DE", ..., can be 0 */
  const char* country;

  /** Modifier/Varint: "Latn", "ije", "latin"..., can be 0 */
  const char* modifier;

  /** Language name: "German", "English", "French", ... */
  const char* name;

  /** Language name in the specified language */
  const char* name_localized;
};

/** Language Definitions */
//*{
static const LanguageSpec languages[] = {
  { "aa", nullptr,    nullptr, "Afar"                        , "ʿAfár af"                 },
  { "af", nullptr,    nullptr, "Afrikaans"                   , "Afrikaans"                },
  { "af", "ZA", nullptr, "Afrikaans (South Africa)"    , nullptr                          },
  { "am", nullptr,    nullptr, "Amharic"                     , "ኣማርኛ"                    },
  { "ar", nullptr,    nullptr, "Arabic"                      , "العربية"                  },
  { "ar", "AR", nullptr, "Arabic (Argentina)"          , nullptr                          },
  { "ar", "OM", nullptr, "Arabic (Oman)"               , nullptr                          },
  { "ar", "SA", nullptr, "Arabic (Saudi Arabia)"       , nullptr                          },
  { "ar", "SY", nullptr, "Arabic (Syrian Arab Republic)", nullptr                         },
  { "ar", "TN", nullptr, "Arabic (Tunisia)"            , nullptr                          },
  { "as", nullptr,    nullptr, "Assamese"                    , "অসমীয়া"                    },
  { "ast",nullptr,    nullptr, "Asturian"                    , "Asturianu"                },
  { "ay", nullptr,    nullptr, "Aymara"                      , "aymar aru"                },
  { "az", nullptr,    nullptr, "Azerbaijani"                 , "Azərbaycanca"             },
  { "az", "IR", nullptr, "Azerbaijani (Iran)"          , nullptr                          },
  { "be", nullptr,    nullptr, "Belarusian"                  , "Беларуская мова"          },
  { "be", nullptr, "latin", "Belarusian"               , "Беларуская мова"          },
  { "bg", nullptr,    nullptr, "Bulgarian"                   , "български"                },
  { "bg", "BG", nullptr, "Bulgarian (Bulgaria)"        , nullptr                          },
  { "bn", nullptr,    nullptr, "Bengali"                     , "বাংলা"                     },
  { "bn", "BD", nullptr, "Bengali (Bangladesh)"        , nullptr                          },
  { "bn", "IN", nullptr, "Bengali (India)"             , nullptr                          },
  { "bo", nullptr,    nullptr, "Tibetan"                     , "བོད་སྐད་"                     },
  { "br", nullptr,    nullptr, "Breton"                      , "brezhoneg"                },
  { "bs", nullptr,    nullptr, "Bosnian"                     , "Bosanski"                 },
  { "bs", "BA", nullptr, "Bosnian (Bosnia/Herzegovina)", nullptr                          },
  { "bs", "BS", nullptr, "Bosnian (Bahamas)"           , nullptr                          },
  { "ca", "ES", "valencia", "Catalan (valencia)" , nullptr                          },
  { "ca", "ES", nullptr, "Catalan (Spain)"             , nullptr                          },
  { "ca", nullptr,    "valencia", "Catalan (valencia)" , nullptr                          },
  { "ca", nullptr,    nullptr, "Catalan"                     , nullptr                          },
  { "cmn", nullptr,    nullptr, "Mandarin"                   , nullptr                          },
  { "co", nullptr,    nullptr, "Corsican"                    , "corsu"                    },
  { "cs", nullptr,    nullptr, "Czech"                       , "Čeština"                  },
  { "cs", "CZ", nullptr, "Czech (Czech Republic)"      , "Čeština (Česká Republika)"},
  { "cy", nullptr,    nullptr, "Welsh"                       , "Welsh"                    },
  { "cy", "GB", nullptr, "Welsh (Great Britain)"       , "Welsh (Great Britain)"    },
  { "cz", nullptr,    nullptr, "Unknown language"            , "Unknown language"         },
  { "da", nullptr,    nullptr, "Danish"                      , "Dansk"                    },
  { "da", "DK", nullptr, "Danish (Denmark)"            , "Dansk (Danmark)"          },
  { "de", nullptr,    nullptr, "German"                      , "Deutsch"                  },
  { "de", "AT", nullptr, "German (Austria)"            , "Deutsch (Österreich)"     },
  { "de", "CH", nullptr, "German (Switzerland)"        , "Deutsch (Schweiz)"        },
  { "de", "DE", nullptr, "German (Germany)"            , "Deutsch (Deutschland)"    },
  { "dk", nullptr,    nullptr, "Unknown language"            , "Unknown language"         },
  { "dz", nullptr,    nullptr, "Dzongkha"                    , "རྫོང་ཁ"                      },
  { "el", nullptr,    nullptr, "Greek"                       , "ελληνικά"                 },
  { "el", "GR", nullptr, "Greek (Greece)"              , nullptr                          },
  { "en", nullptr,    nullptr, "English"                     , "English"                  },
  { "en", "AU", nullptr, "English (Australia)"         , "English (Australia)"      },
  { "en", "CA", nullptr, "English (Canada)"            , "English (Canada)"         },
  { "en", "GB", nullptr, "English (Great Britain)"     , "English (Great Britain)"  },
  { "en", "US", nullptr, "English (United States)"     , "English (United States)"  },
  { "en", "ZA", nullptr, "English (South Africa)"      , "English (South Africa)"   },
  { "en", nullptr, "boldquot", "English"               , "English"                  },
  { "en", nullptr, "quot", "English"                   , "English"                  },
  { "en", "US", "piglatin", "English"            , "English"                  },
  { "eo", nullptr,    nullptr, "Esperanto"                   , "Esperanto"                },
  { "es", nullptr,    nullptr, "Spanish"                     , "Español"                  },
  { "es", "AR", nullptr, "Spanish (Argentina)"         , nullptr                          },
  { "es", "CL", nullptr, "Spanish (Chile)"             , nullptr                          },
  { "es", "CO", nullptr, "Spanish (Colombia)"          , nullptr                          },
  { "es", "CR", nullptr, "Spanish (Costa Rica)"        , nullptr                          },
  { "es", "DO", nullptr, "Spanish (Dominican Republic)", nullptr                          },
  { "es", "EC", nullptr, "Spanish (Ecuador)"           , nullptr                          },
  { "es", "ES", nullptr, "Spanish (Spain)"             , nullptr                          },
  { "es", "GT", nullptr, "Spanish (Guatemala)"         , nullptr                          },
  { "es", "HN", nullptr, "Spanish (Honduras)"          , nullptr                          },
  { "es", "LA", nullptr, "Spanish (Laos)"              , nullptr                          },
  { "es", "MX", nullptr, "Spanish (Mexico)"            , nullptr                          },
  { "es", "NI", nullptr, "Spanish (Nicaragua)"         , nullptr                          },
  { "es", "PA", nullptr, "Spanish (Panama)"            , nullptr                          },
  { "es", "PE", nullptr, "Spanish (Peru)"              , nullptr                          },
  { "es", "PR", nullptr, "Spanish (Puerto Rico)"       , nullptr                          },
  { "es", "SV", nullptr, "Spanish (El Salvador)"       , nullptr                          },
  { "es", "UY", nullptr, "Spanish (Uruguay)"           , nullptr                          },
  { "es", "VE", nullptr, "Spanish (Venezuela)"         , nullptr                          },
  { "et", nullptr,    nullptr, "Estonian"                    , "eesti keel"               },
  { "et", "EE", nullptr, "Estonian (Estonia)"          , nullptr                          },
  { "et", "ET", nullptr, "Estonian (Ethiopia)"         , nullptr                          },
  { "eu", nullptr,    nullptr, "Basque"                      , "euskara"                  },
  { "eu", "ES", nullptr, "Basque (Spain)"              , nullptr                          },
  { "fa", nullptr,    nullptr, "Persian"                     , "فارسى"                    },
  { "fa", "AF", nullptr, "Persian (Afghanistan)"       , nullptr                          },
  { "fa", "IR", nullptr, "Persian (Iran)"              , nullptr                          },
  { "fi", nullptr,    nullptr, "Finnish"                     , "suomi"                    },
  { "fi", "FI", nullptr, "Finnish (Finland)"           , nullptr                          },
  { "fo", nullptr,    nullptr, "Faroese"                     , "Føroyskt"                 },
  { "fo", "FO", nullptr, "Faeroese (Faroe Islands)"    , nullptr                          },
  { "fr", nullptr,    nullptr, "French"                      , "Français"                 },
  { "fr", "CA", nullptr, "French (Canada)"             , "Français (Canada)"        },
  { "fr", "CH", nullptr, "French (Switzerland)"        , "Français (Suisse)"        },
  { "fr", "FR", nullptr, "French (France)"             , "Français (France)"        },
  { "fr", "LU", nullptr, "French (Luxembourg)"         , "Français (Luxembourg)"    },
  { "fy", nullptr,    nullptr, "Frisian"                     , "Frysk"                    },
  { "ga", nullptr,    nullptr, "Irish"                       , "Gaeilge"                  },
  { "gd", nullptr,    nullptr, "Gaelic Scots"                , "Gàidhlig"                 },
  { "gl", nullptr,    nullptr, "Galician"                    , "Galego"                   },
  { "gl", "ES", nullptr, "Galician (Spain)"            , nullptr                          },
  { "gn", nullptr,    nullptr, "Guarani"                     , "Avañe'ẽ"                  },
  { "gu", nullptr,    nullptr, "Gujarati"                    , "ગુજરાતી"                    },
  { "gv", nullptr,    nullptr, "Manx"                        , "Gaelg"                    },
  { "ha", nullptr,    nullptr, "Hausa"                       , "حَوْسَ"                   },
  { "he", nullptr,    nullptr, "Hebrew"                      , "עברית"                     },
  { "he", "IL", nullptr, "Hebrew (Israel)"             , nullptr                          },
  { "hi", nullptr,    nullptr, "Hindi"                       , "हिन्दी"                      },
  { "hi", "IN", nullptr, "Hindi (India)"               , nullptr                          },
  { "hr", nullptr,    nullptr, "Croatian"                    , "Hrvatski"                 },
  { "hr", "HR", nullptr, "Croatian (Croatia)"          , nullptr                          },
  { "hu", nullptr,    nullptr, "Hungarian"                   , "magyar"                   },
  { "hu", "HU", nullptr, "Hungarian (Hungary)"         , nullptr                          },
  { "hy", nullptr,    nullptr, "Armenian"                    , "Հայերեն"                   },
  { "ia", nullptr,    nullptr, "Interlingua"                 , "Interlingua"              },
  { "id", nullptr,    nullptr, "Indonesian"                  , "Bahasa Indonesia"         },
  { "id", "ID", nullptr, "Indonesian (Indonesia)"      , nullptr                          },
  { "is", nullptr,    nullptr, "Icelandic"                   , "Íslenska"                 },
  { "is", "IS", nullptr, "Icelandic (Iceland)"         , nullptr                          },
  { "it", nullptr,    nullptr, "Italian"                     , "Italiano"                 },
  { "it", "CH", nullptr, "Italian (Switzerland)"       , nullptr                          },
  { "it", "IT", nullptr, "Italian (Italy)"             , nullptr                          },
  { "iu", nullptr,    nullptr, "Inuktitut"                   , "ᐃᓄᒃᑎᑐᑦ/inuktitut"         },
  { "ja", nullptr,    nullptr, "Japanese"                    , "日本語"                    },
  { "ja", "JP", nullptr, "Japanese (Japan)"            , nullptr                          },
  { "ka", nullptr,    nullptr, "Georgian"                    , "ქართული"                  },
  { "kk", nullptr,    nullptr, "Kazakh"                      , "Қазақша"                  },
  { "kl", nullptr,    nullptr, "Kalaallisut"                 , "Kalaallisut"              },
  { "km", nullptr,    nullptr, "Khmer"                       , "ភាសាខ្មែរ"                   },
  { "km", "KH", nullptr, "Khmer (Cambodia)"            , nullptr                          },
  { "kn", nullptr,    nullptr, "Kannada"                     , "ಕನ್ನಡ"                      },
  { "ko", nullptr,    nullptr, "Korean"                      , "한국어"                     },
  { "ko", "KR", nullptr, "Korean (Korea)"              , nullptr                          },
  { "ku", nullptr,    nullptr, "Kurdish"                     , "Kurdî"                    },
  { "kw", nullptr,    nullptr, "Cornish"                     , "Kernowek"                 },
  { "ky", nullptr,    nullptr, "Kirghiz"                     , nullptr                          },
  { "la", nullptr,    nullptr, "Latin"                       , "Latina"                   },
  { "lo", nullptr,    nullptr, "Lao"                         , "ລາວ"                       },
  { "lt", nullptr,    nullptr, "Lithuanian"                  , "Lietuvių"                 },
  { "lt", "LT", nullptr, "Lithuanian (Lithuania)"      , nullptr                          },
  { "lv", nullptr,    nullptr, "Latvian"                     , "Latviešu"                 },
  { "lv", "LV", nullptr, "Latvian (Latvia)"            , nullptr                          },
  { "jbo", nullptr,    nullptr, "Lojban"                     , "La .lojban."              },
  { "mg", nullptr,    nullptr, "Malagasy"                    , "Malagasy"                 },
  { "mi", nullptr,    nullptr, "Maori"                       , "Māori"                    },
  { "mk", nullptr,    nullptr, "Macedonian"                  , "Македонски"               },
  { "mk", "MK", nullptr, "Macedonian (Macedonia)"      , nullptr                          },
  { "ml", nullptr,    nullptr, "Malayalam"                   , "മലയാളം"                   },
  { "mn", nullptr,    nullptr, "Mongolian"                   , "Монгол"                   },
  { "mr", nullptr,    nullptr, "Marathi"                     , "मराठी"                      },
  { "ms", nullptr,    nullptr, "Malay"                       , "Bahasa Melayu"            },
  { "ms", "MY", nullptr, "Malay (Malaysia)"            , nullptr                          },
  { "mt", nullptr,    nullptr, "Maltese"                     , "Malti"                    },
  { "my", nullptr,    nullptr, "Burmese"                     , "မြန်မာဘာသာ"                },
  { "my", "MM", nullptr, "Burmese (Myanmar)"           , nullptr                          },
  { "nb", nullptr,    nullptr, "Norwegian Bokmal"            , nullptr                          },
  { "nb", "NO", nullptr, "Norwegian Bokmål (Norway)"   , nullptr                          },
  { "nds", nullptr,   nullptr, "Low German"                  , nullptr                          },
  { "ne", nullptr,    nullptr, "Nepali"                      , nullptr                          },
  { "nl", nullptr,    nullptr, "Dutch"                       , "Nederlands"               },
  { "nl", "BE", nullptr, "Dutch (Belgium)"             , nullptr                          },
  { "nl", "NL", nullptr, "Dutch (Netherlands)"         , nullptr                          },
  { "nn", nullptr,    nullptr, "Norwegian Nynorsk"           , "Norsk nynorsk"            },
  { "nn", "NO", nullptr, "Norwegian Nynorsk (Norway)"  , nullptr                          },
  { "no", nullptr,    nullptr, "Norwegian"                   , "Norsk bokmål"             },
  { "no", "NO", nullptr, "Norwegian (Norway)"          , nullptr                          },
  { "no", "NY", nullptr, "Norwegian (NY)"              , nullptr                          },
  { "nr", nullptr,    nullptr, "Ndebele, South"              , nullptr                          },
  { "oc", nullptr,    nullptr, "Occitan post 1500"           , "Occitan"                  },
  { "om", nullptr,    nullptr, "Oromo"                       , "Oromoo"                   },
  { "or", nullptr,    nullptr, "Oriya"                       , "ଓଡ଼ିଆ"                     },
  { "pa", nullptr,    nullptr, "Punjabi"                     , "ਪੰਜਾਬੀ"                     },
  { "pl", nullptr,    nullptr, "Polish"                      , "Polski"                   },
  { "pl", "PL", nullptr, "Polish (Poland)"             , nullptr                          },
  { "ps", nullptr,    nullptr, "Pashto"                      , "پښتو"                     },
  { "pt", nullptr,    nullptr, "Portuguese"                  , "Português"                },
  { "pt", "BR", nullptr, "Portuguese (Brazil)"         , nullptr                          },
  { "pt", "PT", nullptr, "Portuguese (Portugal)"       , nullptr                          },
  { "qu", nullptr,    nullptr, "Quechua"                     , "Runa Simi"                },
  { "rm", nullptr,    nullptr, "Rhaeto-Romance"              , "Rumantsch"                },
  { "ro", nullptr,    nullptr, "Romanian"                    , "Română"                   },
  { "ro", "RO", nullptr, "Romanian (Romania)"          , nullptr                          },
  { "ru", nullptr,    nullptr, "Russian"                     ,"Русский"                   },
  { "ru", "RU", nullptr, "Russian (Russia"             , nullptr                          },
  { "rw", nullptr,    nullptr, "Kinyarwanda"                 , "Kinyarwanda"              },
  { "sa", nullptr,    nullptr, "Sanskrit"                    , nullptr                          },
  { "sd", nullptr,    nullptr, "Sindhi"                      , nullptr                          },
  { "se", nullptr,    nullptr, "Sami"                        , "Sámegiella"               },
  { "se", "NO", nullptr, "Sami (Norway)"               , nullptr                          },
  { "si", nullptr,    nullptr, "Sinhalese"                   , nullptr                          },
  { "sk", nullptr,    nullptr, "Slovak"                      , "Slovenčina"               },
  { "sk", "SK", nullptr, "Slovak (Slovakia)"           , nullptr                          },
  { "sl", nullptr,    nullptr, "Slovenian"                   , "Slovenščina"              },
  { "sl", "SI", nullptr, "Slovenian (Slovenia)"        , nullptr                          },
  { "sl", "SL", nullptr, "Slovenian (Sierra Leone)"    , nullptr                          },
  { "sm", nullptr,    nullptr, "Samoan"                      , nullptr                          },
  { "so", nullptr,    nullptr, "Somali"                      , nullptr                          },
  { "sp", nullptr,    nullptr, "Unknown language"            , nullptr                          },
  { "sq", nullptr,    nullptr, "Albanian"                    , "Shqip"                    },
  { "sq", "AL", nullptr, "Albanian (Albania)"          , nullptr                          },
  { "sr", nullptr,    nullptr, "Serbian"                     , "Српски / srpski"          },
  { "sr", "YU", nullptr, "Serbian (Yugoslavia)"        , nullptr                          },
  { "sr", nullptr,"ije", "Serbian"                     , nullptr                          },
  { "sr", nullptr, "latin", "Serbian"                  , nullptr                          },
  { "sr", nullptr, "Latn",  "Serbian"                  , nullptr                          },
  { "ss", nullptr,    nullptr, "Swati"                       , nullptr                          },
  { "st", nullptr,    nullptr, "Sotho"                       , nullptr                          },
  { "sv", nullptr,    nullptr, "Swedish"                     , "Svenska"                  },
  { "sv", "SE", nullptr, "Swedish (Sweden)"            , nullptr                          },
  { "sv", "SV", nullptr, "Swedish (El Salvador)"       , nullptr                          },
  { "sw", nullptr,    nullptr, "Swahili"                     , nullptr                          },
  { "ta", nullptr,    nullptr, "Tamil"                       , nullptr                          },
  { "te", nullptr,    nullptr, "Telugu"                      , nullptr                          },
  { "tg", nullptr,    nullptr, "Tajik"                       , nullptr                          },
  { "th", nullptr,    nullptr, "Thai"                        , "ไทย"                      },
  { "th", "TH", nullptr, "Thai (Thailand)"             , nullptr                          },
  { "ti", nullptr,    nullptr, "Tigrinya"                    , nullptr                          },
  { "tk", nullptr,    nullptr, "Turkmen"                     , nullptr                          },
  { "tl", nullptr,    nullptr, "Tagalog"                     , nullptr                          },
  { "to", nullptr,    nullptr, "Tonga"                       , nullptr                          },
  { "tr", nullptr,    nullptr, "Turkish"                     , "Türkçe"                   },
  { "tr", "TR", nullptr, "Turkish (Turkey)"            , nullptr                          },
  { "ts", nullptr,    nullptr, "Tsonga"                      , nullptr                          },
  { "tt", nullptr,    nullptr, "Tatar"                       , nullptr                          },
  { "ug", nullptr,    nullptr, "Uighur"                      , nullptr                          },
  { "uk", nullptr,    nullptr, "Ukrainian"                   , "Українська"               },
  { "uk", "UA", nullptr, "Ukrainian (Ukraine)"         , nullptr                          },
  { "ur", nullptr,    nullptr, "Urdu"                        , nullptr                          },
  { "ur", "PK", nullptr, "Urdu (Pakistan)"             , nullptr                          },
  { "uz", nullptr,    nullptr, "Uzbek"                       , nullptr                          },
  { "uz", nullptr, "cyrillic", "Uzbek"                 , nullptr                          },
  { "vi", nullptr,    nullptr, "Vietnamese"                  , "Tiếng Việt"               },
  { "vi", "VN", nullptr, "Vietnamese (Vietnam)"        , nullptr                          },
  { "wa", nullptr,    nullptr, "Walloon"                     , nullptr                          },
  { "wo", nullptr,    nullptr, "Wolof"                       , nullptr                          },
  { "xh", nullptr,    nullptr, "Xhosa"                       , nullptr                          },
  { "yi", nullptr,    nullptr, "Yiddish"                     , "ייִדיש"                      },
  { "yo", nullptr,    nullptr, "Yoruba"                      , nullptr                          },
  { "zh", nullptr,    nullptr, "Chinese"                     , "中文"                     },
  { "zh", "CN", nullptr, "Chinese (simplified)"        , "中文 (简体)"              },
  { "zh", "HK", nullptr, "Chinese (Hong Kong)"         , "中文 (香港)"              },
  { "zh", "TW", nullptr, "Chinese (traditional)"       , "中文 (繁體)"              },
  { "zu", nullptr,    nullptr, "Zulu"                        , nullptr                          },
  { NULL, nullptr,    nullptr, NULL                          , nullptr                          }
};
//*}

namespace {

std::string
resolve_language_alias(const std::string& name)
{
  typedef std::unordered_map<std::string, std::string> Aliases;
  static Aliases language_aliases;
  if (language_aliases.empty())
  {
    // FIXME: Many of those are not useful for us, since we leave
    // encoding to the app, not to the language, we could/should
    // also match against all language names, not just aliases from
    // locale.alias

    // Aliases taken from /etc/locale.alias
    language_aliases["bokmal"]           = "nb_NO.ISO-8859-1";
    language_aliases["bokmål"]           = "nb_NO.ISO-8859-1";
    language_aliases["catalan"]          = "ca_ES.ISO-8859-1";
    language_aliases["croatian"]         = "hr_HR.ISO-8859-2";
    language_aliases["czech"]            = "cs_CZ.ISO-8859-2";
    language_aliases["danish"]           = "da_DK.ISO-8859-1";
    language_aliases["dansk"]            = "da_DK.ISO-8859-1";
    language_aliases["deutsch"]          = "de_DE.ISO-8859-1";
    language_aliases["dutch"]            = "nl_NL.ISO-8859-1";
    language_aliases["eesti"]            = "et_EE.ISO-8859-1";
    language_aliases["estonian"]         = "et_EE.ISO-8859-1";
    language_aliases["finnish"]          = "fi_FI.ISO-8859-1";
    language_aliases["français"]         = "fr_FR.ISO-8859-1";
    language_aliases["french"]           = "fr_FR.ISO-8859-1";
    language_aliases["galego"]           = "gl_ES.ISO-8859-1";
    language_aliases["galician"]         = "gl_ES.ISO-8859-1";
    language_aliases["german"]           = "de_DE.ISO-8859-1";
    language_aliases["greek"]            = "el_GR.ISO-8859-7";
    language_aliases["hebrew"]           = "he_IL.ISO-8859-8";
    language_aliases["hrvatski"]         = "hr_HR.ISO-8859-2";
    language_aliases["hungarian"]        = "hu_HU.ISO-8859-2";
    language_aliases["icelandic"]        = "is_IS.ISO-8859-1";
    language_aliases["italian"]          = "it_IT.ISO-8859-1";
    language_aliases["japanese"]         = "ja_JP.eucJP";
    language_aliases["japanese.euc"]     = "ja_JP.eucJP";
    language_aliases["ja_JP"]            = "ja_JP.eucJP";
    language_aliases["ja_JP.ujis"]       = "ja_JP.eucJP";
    language_aliases["japanese.sjis"]    = "ja_JP.SJIS";
    language_aliases["korean"]           = "ko_KR.eucKR";
    language_aliases["korean.euc"]       = "ko_KR.eucKR";
    language_aliases["ko_KR"]            = "ko_KR.eucKR";
    language_aliases["lithuanian"]       = "lt_LT.ISO-8859-13";
    language_aliases["no_NO"]            = "nb_NO.ISO-8859-1";
    language_aliases["no_NO.ISO-8859-1"] = "nb_NO.ISO-8859-1";
    language_aliases["norwegian"]        = "nb_NO.ISO-8859-1";
    language_aliases["nynorsk"]          = "nn_NO.ISO-8859-1";
    language_aliases["polish"]           = "pl_PL.ISO-8859-2";
    language_aliases["portuguese"]       = "pt_PT.ISO-8859-1";
    language_aliases["romanian"]         = "ro_RO.ISO-8859-2";
    language_aliases["russian"]          = "ru_RU.ISO-8859-5";
    language_aliases["slovak"]           = "sk_SK.ISO-8859-2";
    language_aliases["slovene"]          = "sl_SI.ISO-8859-2";
    language_aliases["slovenian"]        = "sl_SI.ISO-8859-2";
    language_aliases["spanish"]          = "es_ES.ISO-8859-1";
    language_aliases["swedish"]          = "sv_SE.ISO-8859-1";
    language_aliases["thai"]             = "th_TH.TIS-620";
    language_aliases["turkish"]          = "tr_TR.ISO-8859-9";
  }

  std::string name_lowercase;
  name_lowercase.resize(name.size());
  for(std::string::size_type i = 0; i < name.size(); ++i)
    name_lowercase[i] = static_cast<char>(tolower(name[i]));

  Aliases::iterator i = language_aliases.find(name_lowercase);
  if (i != language_aliases.end())
  {
    return i->second;
  }
  else
  {
    return name;
  }
}

} // namespace

Language
Language::from_spec(const std::string& language, const std::string& country, const std::string& modifier)
{
  typedef std::unordered_map<std::string, std::vector<const LanguageSpec*> > LanguageSpecMap;
  static LanguageSpecMap language_map;

  if (language_map.empty())
  { // Init language_map
    for(int i = 0; languages[i].language != nullptr; ++i)
      language_map[languages[i].language].push_back(&languages[i]);
  }

  LanguageSpecMap::iterator i = language_map.find(language);
  if (i != language_map.end())
  {
    std::vector<const LanguageSpec*>& lst = i->second;

    LanguageSpec tmpspec;
    tmpspec.language = language.c_str();
    tmpspec.country  = country.c_str();
    tmpspec.modifier = modifier.c_str();
    Language tmplang(&tmpspec);

    const LanguageSpec* best_match = nullptr;
    int best_match_score = 0;
    for(std::vector<const LanguageSpec*>::iterator j = lst.begin(); j != lst.end(); ++j)
    { // Search for the language that best matches the given spec, value country more then modifier
      int score = Language::match(Language(*j), tmplang);

      if (score > best_match_score)
      {
        best_match = *j;
        best_match_score = score;
      }
    }
    assert(best_match);
    return Language(best_match);
  }
  else
  {
    return Language();
  }
}

Language
Language::from_name(const std::string& spec_str)
{
  return from_env(resolve_language_alias(spec_str));
}

Language
Language::from_env(const std::string& env)
{
  // Split LANGUAGE_COUNTRY.CODESET@MODIFIER into parts
  std::string::size_type ln = env.find('_');
  std::string::size_type dt = env.find('.');
  std::string::size_type at = env.find('@');

  std::string language;
  std::string country;
  std::string codeset;
  std::string modifier;

  //std::cout << ln << " " << dt << " " << at << std::endl;

  language = env.substr(0, std::min(std::min(ln, dt), at));

  if (ln != std::string::npos && ln+1 < env.size()) // _
  {
    country = env.substr(ln+1, (std::min(dt, at) == std::string::npos) ? std::string::npos : std::min(dt, at) - (ln+1));
  }

  if (dt != std::string::npos && dt+1 < env.size()) // .
  {
    codeset = env.substr(dt+1, (at == std::string::npos) ? std::string::npos : (at - (dt+1)));
  }

  if (at != std::string::npos && at+1 < env.size()) // @
  {
    modifier = env.substr(at+1);
  }

  return from_spec(language, country, modifier);
}

Language::Language(const LanguageSpec* language_spec_)
  : language_spec(language_spec_)
{
}

Language::Language()
  : language_spec()
{
}

int
Language::match(const Language& lhs, const Language& rhs)
{
  if (lhs.get_language() != rhs.get_language())
  {
    return 0;
  }
  else
  {
    static int match_tbl[3][3] = {
      // modifier match, wildchard, miss
      { 9, 8, 5 }, // country match
      { 7, 6, 3 }, // country wildcard
      { 4, 2, 1 }, // country miss
    };

    int c;
    if (lhs.get_country() == rhs.get_country())
      c = 0;
    else if (lhs.get_country().empty() || rhs.get_country().empty())
      c = 1;
    else
      c = 2;

    int m;
    if (lhs.get_modifier() == rhs.get_modifier())
      m = 0;
    else if (lhs.get_modifier().empty() || rhs.get_modifier().empty())
      m = 1;
    else
      m = 2;

    return match_tbl[c][m];
  }
}

std::string
Language::get_language() const
{
  if (language_spec)
    return language_spec->language;
  else
    return "";
}

std::string
Language::get_country()  const
{
  if (language_spec && language_spec->country)
    return language_spec->country;
  else
    return "";
}

std::string
Language::get_modifier() const
{
  if (language_spec && language_spec->modifier)
    return language_spec->modifier;
  else
    return "";
}

std::string
Language::get_name()  const
{
  if (language_spec)
    return language_spec->name;
  else
    return "";
}

std::string
Language::get_localized_name() const
{
  if(language_spec && language_spec->name_localized)
    return language_spec->name_localized;
  else
    return this->get_name();
}

std::string
Language::str() const
{
  if (language_spec)
  {
    std::string var;
    var += language_spec->language;
    if (language_spec->country)
    {
      var += "_";
      var += language_spec->country;
    }

    if (language_spec->modifier)
    {
      var += "@";
      var += language_spec->modifier;
    }
    return var;
  }
  else
  {
    return "";
  }
}

bool
Language::operator==(const Language& rhs) const
{
  return language_spec == rhs.language_spec;
}

bool
Language::operator!=(const Language& rhs) const
{
  return language_spec != rhs.language_spec;
}

} // namespace tinygettext

/* EOF */

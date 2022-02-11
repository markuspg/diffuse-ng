/* Diffuse - A graphical tool for merging and comparing text files
 *
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2022      Markus Prasser
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "df_preferences.h"
#include "df_globals.h"
#include "df_utils.h"

#include <glibmm/convert.h>

#include <algorithm>
#include <locale>
#include <vector>

namespace Df = Diffuse;

Df::Preferences::Preferences(const std::string &path)
    : disable_when{{"display_right_margin",
                    {"display_show_right_margin", false}},
                   {"display_ignore_whitespace_changes",
                    {"display_ignore_whitespace", true}},
                   {"display_ignore_blanklines",
                    {"display_ignore_whitespace", true}},
                   {"display_ignore_endofline",
                    {"display_ignore_whitespace", true}},
                   {"align_ignore_whitespace_changes",
                    {"align_ignore_whitespace", true}},
                   {"align_ignore_blanklines",
                    {"align_ignore_whitespace", true}},
                   {"align_ignore_endofline",
                    {"align_ignore_whitespace", true}}},
      svk_bin{Df::isWindows() ? "svk.bat" : "svk"},

      tmplt{
          "FolderSet",
          {{"Display",
            {FontPref{"display_font", "Monospace 10", "Font"},
             IntPref{"display_tab_width", 8, "Tab width", 1, 1024},
             BoolPref{"display_show_right_margin", true, "Show right margin"},
             IntPref{"display_right_margin", 80, "Right margin", 1, 8192},
             BoolPref{"display_show_line_numbers", true, "Show line numbers"},
             BoolPref{"display_show_whitespace", false,
                      "Show white space characters"},
             BoolPref{"display_ignore_case", false, "Ignore case differences"},
             BoolPref{"display_ignore_whitespace", false,
                      "Ignore white space differences"},
             BoolPref{"display_ignore_whitespace_changes", false,
                      "Ignore changes to white space"},
             BoolPref{"display_ignore_blanklines", false,
                      "Ignore blank line differences"},
             BoolPref{"display_ignore_endofline", false,
                      "Ignore end of line differences"}}},
           {"Alignment",
            {BoolPref{"align_ignore_case", false, "Ignore case"},
             BoolPref{"align_ignore_whitespace", true, "Ignore white space"},
             BoolPref{"align_ignore_whitespace_changes", false,
                      "Ignore changes to white space"},
             BoolPref{"align_ignore_blanklines", false, "Ignore blank lines"},
             BoolPref{"align_ignore_endofline", true,
                      "Ignore end of line characters"}}},
           {"Editor",
            {BoolPref{"editor_auto_indent", true, "Auto indent"},
             BoolPref{"editor_expand_tabs", false, "Expand tabs to spaces"},
             IntPref{"editor_soft_tab_width", 8, "Soft tab width", 1, 1024}}},
           {"Tabs",
            {IntPref{"tabs_default_panes", 2, "Default panes", 2, 16},
             BoolPref{"tabs_always_show", false, "Always show the tab bar"},
             BoolPref{"tabs_warn_before_quit", true,
                      Glib::ustring{"Warn me when closing a tab will quit "} +
                          Df::APP_NAME}}},
           {"Regional Settings",
            {EncodingPref{"encoding_default_codec",
                          "" /* sys.getfilesystemencoding() */,
                          "Default codec"},
             StringPref{"encoding_auto_detect_codecs",
                        "" /* '.join(auto_detect_codecs) */,
                        "Order of codecs used to identify encoding"}}}}} {
  // Find available encodings
  // self.encodings = sorted(set(encodings.aliases.aliases.values()))
  std::vector<Glib::ustring> auto_detect_codecs{"utf_8", "utf_16", "latin_1"};
  const auto e{norm_encoding(Glib::locale_to_utf8(std::locale{""}.name()))};
  if (e && (std::find(auto_detect_codecs.cbegin(), auto_detect_codecs.cend(),
                      e.value()) == auto_detect_codecs.cend())) {
    // Insert after UTF-8 as the default encoding may prevent UTF-8 from being
    // tried
    auto_detect_codecs.emplace(std::cbegin(auto_detect_codecs) + 2, e.value());
  }
}

Glib::ustring Df::Preferences::convertToNativePath(const Glib::ustring &s) {
  return s;
}

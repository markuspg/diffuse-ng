/* Diffuse - A graphical tool for merging and comparing text files
 *
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2022      Markus Prasser
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the license, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  You may also obtain a copy of the GNU General Public License
 * from the Free Software Foundation by visiting their web site
 * (http://www.fsf.org/) or by writing to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "df_preferences.h"
#include "df_font_button.h"

#include <glibmm/convert.h>

namespace Df = Diffuse;

Df::Preferences::Preferences(const std::string &path)
    : disable_when{
          {"display_right_margin", {"display_show_right_margin", false}},
          {"display_ignore_whitespace_changes",
           {"display_ignore_whitespace", true}},
          {"display_ignore_blanklines", {"display_ignore_whitespace", true}},
          {"display_ignore_endofline", {"display_ignore_whitespace", true}},
          {"align_ignore_whitespace_changes",
           {"align_ignore_whitespace", true}},
          {"align_ignore_blanklines", {"align_ignore_whitespace", true}},
          {"align_ignore_endofline", {"align_ignore_whitespace", true}}} {}

std::string Df::Preferences::convertToNativePath(const Glib::ustring &s) {
  return Glib::locale_from_utf8(s);
}

bool Df::Preferences::getBool(const Glib::ustring &name) {
  return bool_prefs.at(name);
}

int Df::Preferences::getInt(const Glib::ustring &name) {
  return int_prefs.at(name);
}

Glib::ustring Df::Preferences::getString(const Glib::ustring &name) {
  return string_prefs.at(name);
}

void Df::Preferences::setBool(const Glib::ustring &name, const bool value) {
  bool_prefs[name] = value;
}

void Df::Preferences::setString(const Glib::ustring &name,
                                const Glib::ustring &value) {
  string_prefs[name] = value;
}

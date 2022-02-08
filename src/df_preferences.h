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

#ifndef DF_PREFERENCES_H
#define DF_PREFERENCES_H

#include <glibmm/ustring.h>

#include <map>

namespace Diffuse {
/**
 * @brief Class to store preferences and to construct a dialog for manipulating
 *   them
 */
class Preferences {
public:
  Preferences(const std::string &path);

  bool getBool(const Glib::ustring &name) { return bool_prefs[name]; }
  Glib::ustring getString(const Glib::ustring &name) {
    return string_prefs[name];
  }
  void setBool(const Glib::ustring &name, bool value) {
    bool_prefs[name] = value;
  }
  void setString(const Glib::ustring &name, const Glib::ustring &value) {
    string_prefs[name] = value;
  }

  Glib::ustring convertToNativePath(const Glib::ustring &s);

private:
  std::map<Glib::ustring, bool> bool_prefs;
  std::map<Glib::ustring, int> int_prefs;
  std::map<Glib::ustring, int> int_prefs_max;
  std::map<Glib::ustring, int> int_prefs_min;
  std::map<Glib::ustring, Glib::ustring> string_prefs;
  std::map<Glib::ustring, void *> string_prefs_enums;

  const Glib::ustring svk_bin;
};
} // namespace Diffuse

#endif // DF_PREFERENCES_H

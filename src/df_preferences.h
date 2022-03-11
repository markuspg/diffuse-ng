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

#ifndef DF_PREFERENCES_H
#define DF_PREFERENCES_H

#include <glibmm/ustring.h>

#include <map>

namespace Diffuse {
/**
 * @brief Class to store preferences and construct a dialogue for manipulating
 *   them
 */
class Preferences {
public:
  Preferences(const std::string &path);

  std::string convertToNativePath(const Glib::ustring &s);
  bool getBool(const Glib::ustring &name);
  int getInt(const Glib::ustring &name);
  Glib::ustring getString(const Glib::ustring &name);
  void setBool(const Glib::ustring &name, bool value);
  void setString(const Glib::ustring &name, const Glib::ustring &value);

private:
  struct DisableWhenCondition {
    Glib::ustring option;
    bool state;
  };

  std::map<Glib::ustring, bool> bool_prefs;
  std::map<Glib::ustring, int> int_prefs;
  std::map<Glib::ustring, int> int_prefs_min;
  std::map<Glib::ustring, int> int_prefs_max;
  std::map<Glib::ustring, Glib::ustring> string_prefs;
  std::map<Glib::ustring, Glib::ustring> string_prefs_enums;

  //! Conditions to determine if a preference should be greyed out
  std::map<Glib::ustring, DisableWhenCondition> disable_when;
};
} // namespace Diffuse

#endif // DF_PREFERENCES_H

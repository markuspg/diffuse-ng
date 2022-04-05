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
#include <variant>
#include <vector>

namespace Diffuse {
/**
 * @brief Class to store preferences and construct a dialogue for manipulating
 *   them
 */
class Preferences {
public:
  struct Preference {
    Glib::ustring name;
    Glib::ustring label;
  };

  struct BoolPreference : public Preference {
    BoolPreference(const Glib::ustring &name, const bool dflt,
                   const Glib::ustring &label);

    const bool dflt;
  };
  struct EncodingPreference : public Preference {
    EncodingPreference(const Glib::ustring &name, const Glib::ustring &dflt,
                       const Glib::ustring &label);

    const Glib::ustring dflt;
  };
  struct FilePreference : public Preference {};
  struct FontPreference : public Preference {
    FontPreference(const Glib::ustring &name, const Glib::ustring &dflt,
                   const Glib::ustring &label);

    const Glib::ustring dflt;
  };
  struct IntPreference : public Preference {
    IntPreference(const Glib::ustring &name, int dflt,
                  const Glib::ustring &label, int min, int max);

    const int dflt;
    const int min;
    const int max;
  };
  struct StringPreference : public Preference {
    StringPreference(const Glib::ustring &name, const Glib::ustring &dflt,
                     const Glib::ustring &label);

    const Glib::ustring dflt;
  };

  struct Category {
    Glib::ustring name;
    std::vector<std::variant<BoolPreference, EncodingPreference, FilePreference,
                             FontPreference, IntPreference, StringPreference>>
        preferences;
  };

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

  //! Describe a preferenec dialogue, its layout and default values
  std::vector<Category> templt;
};
} // namespace Diffuse

#endif // DF_PREFERENCES_H

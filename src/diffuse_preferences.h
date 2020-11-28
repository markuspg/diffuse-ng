/*
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2020      Markus Prasser <markuspg@users.noreply.github.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the licence, or (at your option) any later
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

#ifndef DIFFUSE_PREFERENCES_H
#define DIFFUSE_PREFERENCES_H

#include <glibmm/ustring.h>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <map>
#include <memory>
#include <vector>

namespace Diffuse {
/*!
 * \brief Class to store preferences and construct a dialogue for manipulating
 * them
 */
class Preferences {
public:
  Preferences(const Glib::ustring &path);

  Glib::ustring convertToNativePath(const Glib::ustring &s);
  bool getBool(const Glib::ustring &name) const;
  Glib::ustring getDefaultEncoding() const;
  std::vector<Glib::ustring> getEncodings() const;
  int getInt(const Glib::ustring &name) const;
  Glib::ustring getString(const Glib::ustring &name) const;
  void setBool(const Glib::ustring &name, bool value);

private:
  struct BaseOpt {
    virtual ~BaseOpt(){};
  };
  struct BoolOpt : BaseOpt {
    BoolOpt(const Glib::ustring &nm, bool def, const Glib::ustring &lbl)
        : name{nm}, deflt{def}, label{lbl} {}

    const Glib::ustring name;
    const bool deflt;
    const Glib::ustring label;
  };
  struct EncodingOpt : BaseOpt {
    EncodingOpt(const Glib::ustring &nm, const Glib::ustring &def,
                const Glib::ustring &lbl)
        : name{nm}, deflt{def}, label{lbl} {}

    const Glib::ustring name;
    const Glib::ustring deflt;
    const Glib::ustring label;
  };
  struct FileOpt : BaseOpt {
    FileOpt(const Glib::ustring &nm, const boost::optional<Glib::ustring> &def,
            const Glib::ustring &lbl)
        : name{nm}, deflt{def}, label{lbl} {}

    const Glib::ustring name;
    const boost::optional<Glib::ustring> deflt;
    const Glib::ustring label;
  };
  struct FontOpt : BaseOpt {
    FontOpt(const Glib::ustring &nm, const Glib::ustring &def,
            const Glib::ustring &lbl)
        : name{nm}, deflt{def}, label{lbl} {}

    const Glib::ustring name;
    const Glib::ustring deflt;
    const Glib::ustring label;
  };
  struct IntegerOpt : BaseOpt {
    IntegerOpt(const Glib::ustring &nm, int def, const Glib::ustring &lbl,
               int mn, int mx)
        : name{nm}, deflt{def}, label{lbl}, min{mn}, max{mx} {}

    const Glib::ustring name;
    const int deflt;
    const Glib::ustring label;
    const int min;
    const int max;
  };
  struct StringOpt : BaseOpt {
    StringOpt(const Glib::ustring &nm, const Glib::ustring &def,
              const Glib::ustring &lbl)
        : name{nm}, deflt{def}, label{lbl} {}

    const Glib::ustring name;
    const Glib::ustring deflt;
    const Glib::ustring label;
  };

  using List = std::vector<std::shared_ptr<BaseOpt>>;
  struct Category {
    const Glib::ustring label;
    List list;
  };
  using FolderSet = std::vector<Category>;

  std::vector<Glib::ustring> getDefaultEncodings() const;

  std::map<Glib::ustring, bool> bool_prefs;
  std::map<Glib::ustring, int> int_prefs;
  std::map<Glib::ustring, int> int_prefs_min;
  std::map<Glib::ustring, int> int_prefs_max;
  const std::vector<Glib::ustring> encodings;
  std::map<Glib::ustring, Glib::ustring> string_prefs;
  std::map<void *, void *> string_prefs_enums;

  //! Conditions used to determine if a preference should be greyed out
  const std::map<Glib::ustring, std::pair<Glib::ustring, bool>> disable_when;

  /*!
   * \brief tmplt describes how preference dialogue layout
   *
   * Zhis will be traversed later to build the preferences dialogue and discover
   * which preferences exist.
   *
   * Folders are described using:
   *   [ 'FolderSet', label1, template1, label2, template2, ... ]
   * Lists are described using:
   *   [ 'List', template1, template2, template3, ... ]
   * Individual preferences are described using one of the following depending
   * upon its type and the desired widget:
   *   [ 'Boolean', name, default, label ]
   *   [ 'Integer', name, default, label ]
   *   [ 'String', name, default, label ]
   *   [ 'File', name, default, label ]
   *   [ 'Font', name, default, label ]
   */
  FolderSet tmplt;
};
} // namespace Diffuse

#endif // DIFFUSE_PREFERENCES_H

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

#ifndef DF_DIFFUSE_H
#define DF_DIFFUSE_H

#include "df_preferences.h"

#include <optional>
#include <variant>
#include <vector>

namespace Diffuse {
using Encoding = std::optional<Glib::ustring>;
using Labels = std::vector<Glib::ustring>;
using Options =
    std::map<Glib::ustring, std::variant<Glib::ustring, unsigned long>>;
struct Revision {
  Revision(const std::optional<Glib::ustring> &rev,
           std::optional<Glib::ustring> &enc)
      : revision{rev}, encoding{enc} {}

  std::optional<Glib::ustring> revision;
  Encoding encoding;
};
using Revisions = std::vector<Revision>;
struct Specification {
  Specification(const std::optional<std::string> &fn, const Revisions &revs)
      : filename{fn}, revisions{revs} {}

  std::optional<std::string> filename;
  Revisions revisions;
};

using Specs = std::vector<Specification>;

class Diffuse {
public:
  Diffuse(const Glib::ustring &rc_dir);

  void createCommitFileTabs(const Specs &items, const Labels &labels,
                            const Options &options);
  void createModifiedFileTabs(const Specs &items, const Labels &labels,
                              const Options &options);
  void createSeparateTabs(const Specs &items, const Labels &labels,
                          const Options &options);
  void createSingleTab(const Specs &items, const Labels &labels,
                       const Options &options);

  bool loadState(const std::string &statepath);
  void preferences_updated();

  Preferences prefs;
};

using TabCreationFunc = void (Diffuse::*)(const Specs &, const Labels &,
                                          const Options &);
using TabCreationFuncMap = std::map<Glib::ustring, TabCreationFunc>;
} // namespace Diffuse

#endif // DF_DIFFUSE_H

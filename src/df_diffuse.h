/* Diffuse - a graphical tool for merging and comparing text files
 * Copyright (C) 2022 Markus Prasser
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef DF_DIFFUSE_H
#define DF_DIFFUSE_H

#include "df_preferences.h"

#include <gtkmm/notebook.h>
#include <gtkmm/window.h>

#include <optional>
#include <variant>

namespace Diffuse {
using Encoding = std::optional<Glib::ustring>;
using Labels = std::vector<Glib::ustring>;
using Options =
    std::map<Glib::ustring, std::variant<Glib::ustring, unsigned long>>;
struct Revision {
  Revision(const std::optional<Glib::ustring> &rev, const Encoding &enc)
      : revision{rev}, encoding{enc} {}
  std::optional<Glib::ustring> revision;
  Encoding encoding;
};
using Revs = std::vector<Revision>;
struct Specification {
  Specification(const std::optional<Glib::ustring> &fn, const Revs &revs)
      : filename{fn}, revisions{revs} {}
  std::optional<Glib::ustring> filename;
  Revs revisions;
};
using Specs = std::vector<Specification>;

class Diffuse : Gtk::Window {
public:
  Diffuse(const Glib::ustring &rc_dir);

  void closeOnSame();
  void createCommitFileTabs(const Specs &items, const Labels &labels,
                            const Options &options) {}
  void createModifiedFileTabs(const Specs &items, const Labels &labels,
                              const Options &options) {}
  void createSeparateTabs(const Specs &items, const Labels &labels,
                          const Options &options) {}
  void createSingleTab(const Specs &items, const Labels &labels,
                       const Options &options) {}
  bool loadState(const Glib::ustring &statepath) { return true; }
  void preferences_updated() {}
  void saveState(const Glib::ustring &statepath);

  Gtk::Notebook notebook;
  Preferences prefs;
};
} // namespace Diffuse

#endif // DF_DIFFUSE_H

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

#ifndef DIFFUSE_WINDOW_H
#define DIFFUSE_WINDOW_H

#include "diffuse_preferences.h"

#include <gtkmm/window.h>

#include <boost/optional.hpp>
#include <boost/variant.hpp>

#include <map>

namespace Gtk {
class Notebook;
} // namespace Gtk

namespace Diffuse {
class Window : public Gtk::Window {
public:
  using Options = std::map<Glib::ustring, boost::variant<Glib::ustring, int>>;
  using Revs = std::vector<std::pair<boost::optional<Glib::ustring>,
                                     boost::optional<Glib::ustring>>>;
  using Specs = std::vector<std::pair<boost::optional<Glib::ustring>, Revs>>;

  Window(const Glib::ustring &rc_dir);

  void closeOnSame();
  void createCommitFileTabs(const Specs &, const std::vector<Glib::ustring> &,
                            const Options &);
  void createModifiedFileTabs(const Specs &, const std::vector<Glib::ustring> &,
                              const Options &);
  void createSeparateTabs(const Specs &, const std::vector<Glib::ustring> &,
                          const Options &);
  void createSingleTab(const Specs &, const std::vector<Glib::ustring> &,
                       const Options &);
  bool loadState(const Glib::ustring &statepath);
  void newLoadedFileDiffViewer(void *);
  void preferences_updated();
  bool saveState(const Glib::ustring &statepath);

  Gtk::Notebook *notebook = nullptr;
  Preferences prefs;
};
} // namespace Diffuse

#endif // DIFFUSE_WINDOW_H

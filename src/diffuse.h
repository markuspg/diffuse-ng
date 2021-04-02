/*
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2021      Markus Prasser <markuspg@gmx.de>
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

#ifndef DIFFUSE_DIFFUSE_H_
#define DIFFUSE_DIFFUSE_H_

#include "labels.h"
#include "option.h"
#include "preferences.h"
#include "spec.h"

#include <gtkmm/notebook.h>
#include <gtkmm/window.h>

namespace Diffuse {
class Diffuse : public Gtk::Window {
public:
  Diffuse(const Glib::ustring &rc_dir);

  void closeOnSame();
  void createCommitFileTabs(const Specs &items, const Labels &labels,
                            const Options &options);
  void createModifiedFileTabs(const Specs &items, const Labels &labels,
                              const Options &options);
  void createSeparateTabs(const Specs &items, const Labels &labels,
                          const Options &options);
  void createSingleTab(const Specs &items, const Labels &labels,
                       const Options &options);
  void loadState(const Glib::ustring &statepath);
  void preferences_updated();
  void saveState(const Glib::ustring &statepath);

  Gtk::Notebook notebook;
  Preferences prefs;
};
} // namespace Diffuse

#endif // DIFFUSE_DIFFUSE_H_

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

#include "diffuse_window.h"

#include <glibmm/miscutils.h>
#include <gtkmm/box.h>
#include <gtkmm/notebook.h>

namespace Df = Diffuse;

static Df::Window::Items
assign_file_labels(const Df::Window::Specs &items,
                   const std::vector<Glib::ustring> &labels);

Df::Window::Window(const Glib::ustring &rc_dir)
    : prefs{Glib::build_path(G_DIR_SEPARATOR_S,
                             std::vector<Glib::ustring>{rc_dir, "prefs"})} {
  // Create a VBox for our contents
  const auto vbox = manage(new Gtk::VBox);

  notebook = manage(new Gtk::Notebook);
  notebook->set_scrollable(true);
  // TODO: notebook.connect('switch-page', self.switch_page_cb)
  vbox->pack_start(*notebook, true, true, 0);
  notebook->show();

  add(*vbox);
  vbox->show();
}

void Df::Window::closeOnSame() {}

void Df::Window::createCommitFileTabs(const Specs &,
                                      const std::vector<Glib::ustring> &,
                                      const Options &) {}

void Df::Window::createModifiedFileTabs(const Specs &,
                                        const std::vector<Glib::ustring> &,
                                        const Options &) {}

/*!
 * \brief Create a new viewer for each item in "items"
 */
void Df::Window::createSeparateTabs(const Specs &items,
                                    const std::vector<Glib::ustring> &labels,
                                    const Options &options) {
  // All tabs inherit the first tab's revision and encoding specifications
  Specs tmp_items;
  for (const auto &item : items) {
    tmp_items.emplace_back(item.first, items.front().second);
  }
  const auto tmp = assign_file_labels(tmp_items, labels);
  for (const auto &item : tmp) {
    newLoadedFileDiffViewer({item}); // TODO: setOptions(options)
  }
}

/*!
 * \brief Create a new viewer for "items"
 */
void Df::Window::createSingleTab(const Specs &items,
                                 const std::vector<Glib::ustring> &labels,
                                 const Options &options) {
  if (items.size() > 0) {
    newLoadedFileDiffViewer(
        assign_file_labels(items, labels)); // TODO: .setOptions(options)
  }
}

bool Df::Window::loadState(const Glib::ustring &statepath) { return true; }

/*!
 * \brief Create a new viewer to display "items"
 */
void Df::Window::newLoadedFileDiffViewer(const Items &items) {}

void Df::Window::preferences_updated() {}

bool Df::Window::saveState(const Glib::ustring &statepath) {}

/*!
 * \brief Assign user specified labels to the corresponding files
 */
static Df::Window::Items
assign_file_labels(const Df::Window::Specs &items,
                   const std::vector<Glib::ustring> &labels) {
  Df::Window::Items new_items;
  std::vector<Glib::ustring> ss{labels};
  std::reverse(ss.begin(), ss.end());

  for (const auto &item : items) {
    boost::optional<Glib::ustring> s;
    if (false == ss.empty()) {
      s = ss.back();
      ss.pop_back();
    }
    new_items.emplace_back(Df::Window::Item{item.first, item.second, s});
  }

  return new_items;
}

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

void Df::Window::createSeparateTabs(const Specs &,
                                    const std::vector<Glib::ustring> &,
                                    const Options &) {}

void Df::Window::createSingleTab(const Specs &,
                                 const std::vector<Glib::ustring> &,
                                 const Options &) {}

bool Df::Window::loadState(const Glib::ustring &statepath) { return true; }

void Df::Window::newLoadedFileDiffViewer(void *) {}

void Df::Window::preferences_updated() {}

bool Df::Window::saveState(const Glib::ustring &statepath) {}

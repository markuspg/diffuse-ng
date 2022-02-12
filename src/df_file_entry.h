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

#ifndef DF_FILE_ENTRY_H
#define DF_FILE_ENTRY_H

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/image.h>

namespace Diffuse {
/**
 * @brief Text entry widget with a button to help pick file names
 */
class FileEntry : public Gtk::HBox {
public:
  FileEntry(Gtk::Window &parent, const Glib::ustring &title);

  Glib::ustring get_text() const { return entry.get_text(); };
  void set_text(const Glib::ustring &s) { entry.set_text(s); }

private:
  void chooseFile();

  Gtk::Button button;
  Gtk::Entry entry;
  Gtk::Image image;
  const Glib::ustring title;
  Gtk::Window &toplevel;
};
} // namespace Diffuse

#endif // DF_FILE_ENTRY_H

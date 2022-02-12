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

#include "df_file_entry.h"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>

namespace Df = Diffuse;

Df::FileEntry::FileEntry(Gtk::Window &parent, const Glib::ustring &title)
    : title{title}, toplevel{parent} {
  pack_start(entry, true, true, 0);
  entry.show();
  gtk_image_set_from_stock(image.gobj(), GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
  button.add(image);
  image.show();
  button.signal_clicked().connect(sigc::mem_fun(this, &FileEntry::chooseFile));
  pack_start(button, false, false, 0);
  button.show();
}

/**
 * @brief Action performed when the pick file button is pressed
 */
void Df::FileEntry::chooseFile() {
  Gtk::FileChooserDialog dialog{toplevel, title, Gtk::FILE_CHOOSER_ACTION_OPEN};
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);
  if (Gtk::RESPONSE_OK == dialog.run()) {
    entry.set_text(dialog.get_filename());
  }
}

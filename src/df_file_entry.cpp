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

#include "df_file_entry.h"

#include <gtkmm/filechooserdialog.h>
#include <gtkmm/stock.h>

namespace Df = Diffuse;

Df::FileEntry::FileEntry(const Glib::RefPtr<Gtk::Window> &parent,
                         const Glib::ustring &ttl)
    : title{ttl}, toplevel{parent} {
  pack_start(entry, true, true, 0);
  entry.show();

  Gtk::Button button;
  Gtk::Image image;
  gtk_image_set_from_stock(image.gobj(), GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
  button.add(image);
  image.show();
  button.signal_clicked().connect(sigc::mem_fun(*this, &FileEntry::chooseFile));
  pack_start(button, false, false, 0);
  button.show();
}

/**
 * @brief Action performed when the pick file button is pressed
 */
void Df::FileEntry::chooseFile() {
  std::unique_ptr<Gtk::FileChooserDialog> dialog;
  if (toplevel) {
    dialog = std::make_unique<Gtk::FileChooserDialog>(
        *(toplevel.get()), title,
        Gtk::FileChooserAction::FILE_CHOOSER_ACTION_OPEN);
  } else {
    dialog = std::make_unique<Gtk::FileChooserDialog>(
        title, Gtk::FileChooserAction::FILE_CHOOSER_ACTION_OPEN);
  }

  dialog->add_button(Gtk::Stock::CANCEL, Gtk::ResponseType::RESPONSE_CANCEL);
  dialog->add_button(Gtk::Stock::OPEN, Gtk::ResponseType::RESPONSE_OK);

  const auto current_dir{Glib::get_current_dir()};
  auto tmp_path{g_canonicalize_filename(current_dir.c_str(), nullptr)};
  dialog->set_current_folder(Glib::locale_to_utf8(tmp_path));
  g_free(tmp_path);
  tmp_path = nullptr;

  if (Gtk::ResponseType::RESPONSE_OK == dialog->run()) {
    entry.set_text(dialog->get_filename());
  }
}

Glib::ustring Df::FileEntry::get_text() const { return entry.get_text(); }

void Df::FileEntry::set_text(const Glib::ustring &s) { entry.set_text(s); }

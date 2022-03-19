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

#include "df_notebook_tab.h"

#include <gtkmm/box.h>
#include <gtkmm/image.h>

#include <glib/gi18n.h>

namespace Df = Diffuse;

Df::NotebookTab::NotebookTab(const Glib::ustring &name,
                             const std::optional<Gtk::StockID> &stock)
    : label{name} {
  set_visible_window(false);

  Gtk::HBox hbox;

  if (stock) {
    Gtk::Image image;
    gtk_image_set_from_stock(image.gobj(), stock.value().get_c_str(),
                             GTK_ICON_SIZE_MENU);
    hbox.pack_start(image, false, false, 5);
    image.show();
  }

  // Left-justify the widget
  label.set_alignment(0, 0.5f);
  hbox.pack_start(label, true, true, 0);
  label.show();

  button.set_relief(Gtk::RELIEF_NONE);
  Gtk::Image image;
  gtk_image_set_from_stock(image.gobj(), GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  button.add(image);
  image.show();
  button.set_tooltip_text(_("Close Tab"));
  hbox.pack_start(button, false, false, 0);
  button.show();

  add(hbox);
  hbox.show();
}

Glib::ustring Df::NotebookTab::get_text() const { return label.get_text(); }

void Df::NotebookTab::set_text(const Glib::ustring &s) { label.set_text(s); }

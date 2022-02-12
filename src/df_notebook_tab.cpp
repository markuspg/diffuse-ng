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

#include "df_notebook_tab.h"

#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/stock.h>

namespace Df = Diffuse;

Df::NotebookTab::NotebookTab(const Glib::ustring &name,
                             const std::optional<Gtk::BuiltinStockID> &stock)
    : label{name} {
  set_visible_window(false);

  Gtk::HBox hbox;

  if (stock) {
    Gtk::Image image;
    gtk_image_set_from_stock(image.gobj(), stock->id, GTK_ICON_SIZE_MENU);
    hbox.pack_start(image, false, false, 5);
    image.show();
  }

  // Left justify the widget
  label.set_alignment(0, 0.5f);
  hbox.pack_start(label, true, true, 0);
  label.show();

  button.set_relief(Gtk::RELIEF_NONE);
  Gtk::Image image;
  gtk_image_set_from_stock(image.gobj(), GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  button.add(image);
  image.show();
  set_tooltip_text("Close Tab"); // TODO: not set_tooltip?
  hbox.pack_start(button, false, false, 0);
  button.show();

  add(hbox);
  hbox.show();
}

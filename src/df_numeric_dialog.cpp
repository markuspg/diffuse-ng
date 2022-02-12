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

#include "df_numeric_dialog.h"

#include <gtkmm/adjustment.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>

namespace Df = Diffuse;

Df::NumericDialog::NumericDialog(Gtk::Window &parent,
                                 const Glib::ustring &title,
                                 const Glib::ustring &text, const double val,
                                 const double lower, const double upper,
                                 const double step, const double page)
    : Gtk::Dialog{title, parent, true}, adj{val, lower, upper, step, page},
      button{adj, 0} {
  add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_REJECT);
  add_button(Gtk::Stock::OK, Gtk::RESPONSE_ACCEPT);

  Gtk::VBox vbox;
  vbox.set_border_width(10);

  Gtk::HBox hbox;
  Gtk::Label label{text};
  hbox.pack_start(label, false, false, 0);
  label.show();

  button.signal_activate().connect(
      sigc::mem_fun(this, &NumericDialog::button_cb));
  hbox.pack_start(button, true, true, 0);
  button.show();

  vbox.pack_start(hbox, true, true, 0);
  hbox.show();

  get_vbox()->pack_start(vbox, false, false, 0);
  vbox.show();
}

void Df::NumericDialog::button_cb() { response(Gtk::RESPONSE_ACCEPT); }

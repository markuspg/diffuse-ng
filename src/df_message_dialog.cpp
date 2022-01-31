/* Diffuse - a graphical tool for merging and comparing text files
 * Copyright (C) 2022 Markus Prasser
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "df_message_dialog.h"
#include "df_globals.h"

namespace Df = Diffuse;

Df::MessageDialog::MessageDialog(const Gtk::MessageType type,
                                 const Glib::ustring &s)
    : Gtk::MessageDialog{s, false, type,
                         type == Gtk::MESSAGE_ERROR ? Gtk::BUTTONS_OK
                                                    : Gtk::BUTTONS_OK_CANCEL,
                         true} {
  set_title(APP_NAME);
}

Df::MessageDialog::MessageDialog(Gtk::Window &parent,
                                 const Gtk::MessageType type,
                                 const Glib::ustring &s)
    : Gtk::MessageDialog{parent,
                         s,
                         false,
                         type,
                         type == Gtk::MESSAGE_ERROR ? Gtk::BUTTONS_OK
                                                    : Gtk::BUTTONS_OK_CANCEL,
                         true} {
  set_title(APP_NAME);
}

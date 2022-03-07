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

#include "df_message_dialog.h"
#include "df_globals.h"

namespace Df = Diffuse;

Df::MessageDialog::MessageDialog(const Gtk::MessageType type,
                                 const Glib::ustring &s)
    : Gtk::MessageDialog{s, false, type,
                         Gtk::MessageType::MESSAGE_ERROR == type
                             ? Gtk::ButtonsType::BUTTONS_OK
                             : Gtk::ButtonsType::BUTTONS_OK_CANCEL,
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
                         Gtk::MessageType::MESSAGE_ERROR == type
                             ? Gtk::ButtonsType::BUTTONS_OK
                             : Gtk::ButtonsType::BUTTONS_OK_CANCEL,
                         true} {
  set_title(APP_NAME);
}

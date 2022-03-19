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

#ifndef DF_NOTEBOOK_TAB_H
#define DF_NOTEBOOK_TAB_H

#include <gtkmm/button.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>

#include <optional>

namespace Diffuse {
/**
 * @brief Widget class to create notebook tabs with labels and a close button
 *
 * Use button.connect() to be notified when the button is pressed. This is made
 * a Gtk::EventBox so signals can be connected for MMB and RMB button presses.
 */
class NotebookTab : Gtk::EventBox {
public:
  NotebookTab(const Glib::ustring &name,
              const std::optional<Gtk::StockID> &stock);

  Glib::ustring get_text() const;
  void set_text(const Glib::ustring &s);

  Gtk::Button button;

private:
  Gtk::Label label;
};
} // namespace Diffuse

#endif // DF_NOTEBOOK_TAB_H

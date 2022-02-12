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

#ifndef DF_NOTEBOOK_TAB_H
#define DF_NOTEBOOK_TAB_H

#include <gtkmm/button.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/label.h>
#include <gtkmm/stockid.h>

#include <optional>

namespace Diffuse {
/**
 * @brief Widget class to create notebook tabs with labels and a close button
 *
 * Use notebooktab.button.connect() to be notified when the button is pressed.
 * Make this a gtk.EventBox so signals can be connected for MMB and RMB button
 * presses.
 */
class NotebookTab : public Gtk::EventBox {
public:
  NotebookTab(const Glib::ustring &name,
              const std::optional<Gtk::BuiltinStockID> &stock);

  Glib::ustring get_text() const { return label.get_text(); }
  void set_text(const Glib::ustring &s) { label.set_text(s); }

  Gtk::Button button;

private:
  Gtk::Label label;
};
} // namespace Diffuse

#endif // DF_NOTEBOOK_TAB_H

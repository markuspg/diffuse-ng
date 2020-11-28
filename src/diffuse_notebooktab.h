/*
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2020      Markus Prasser <markuspg@users.noreply.github.com>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the licence, or (at your option) any later
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

#ifndef DIFFUSE_NOTEBOOKTAB_H
#define DIFFUSE_NOTEBOOKTAB_H

#include <gtkmm/eventbox.h>

#include <boost/optional.hpp>

namespace Gtk {
class Button;
class Label;
class StockID;
} // namespace Gtk

namespace Diffuse {
/*!
 * \brief Widget classed to create notebook tabs with labels and a close button
 *
 * Use notebooktab.button.connect() to be notified when the button is pressed.
 * Make this a Gtk::EventBox so signals can be connected for MMB and RMB button
 * presses.
 */
class NotebookTab : public Gtk::EventBox {
public:
  NotebookTab(const Glib::ustring &name,
              const boost::optional<Gtk::StockID> &stock);

  Glib::ustring get_text() const;
  void set_text(const Glib::ustring &s);

private:
  Gtk::Button *button = nullptr;
  Gtk::Label *label = nullptr;
};
} // namespace Diffuse

#endif // DIFFUSE_NOTEBOOKTAB_H

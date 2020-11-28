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

#ifndef DIFFUSE_ENCODINGMENU_H
#define DIFFUSE_ENCODINGMENU_H

#include <gtkmm/box.h>

#include <boost/optional.hpp>

#include <vector>

namespace Gtk {
class ComboBoxText;
} // namespace Gtk

namespace Diffuse {
class Preferences;

/*!
 * \brief Widget to help pick an encoding
 */
class EncodingMenu : public Gtk::HBox {
public:
  EncodingMenu(const Preferences &prefs, bool autodetect = false);

  boost::optional<Glib::ustring> get_text() const;
  static boost::optional<Glib::ustring>
  norm_encoding(const boost::optional<Glib::ustring> &e);
  void set_text(const Glib::ustring encoding);

private:
  Gtk::ComboBoxText *combobox = nullptr;
  std::vector<boost::optional<Glib::ustring>> encodings;
};
} // namespace Diffuse

#endif // DIFFUSE_ENCODINGMENU_H

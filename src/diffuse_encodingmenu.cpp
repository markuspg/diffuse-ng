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

#include "diffuse_encodingmenu.h"
#include "diffuse_preferences.h"

#include <glibmm/i18n.h>
#include <gtkmm/comboboxtext.h>

namespace Df = Diffuse;

Df::EncodingMenu::EncodingMenu(const Preferences &prefs, bool autodetect) {
  combobox = manage(new Gtk::ComboBoxText);

  for (const auto &e : prefs.getEncodings()) {
    encodings.emplace_back(e);
  }

  for (const auto &e : encodings) {
    combobox->append_text(boost::get(e));
  }

  if (autodetect) {
    encodings.emplace(encodings.cbegin(), boost::none);
    combobox->prepend_text(_("Auto Detect"));
  }

  pack_start(*combobox, false, false, 0);
  combobox->show();
}

boost::optional<Glib::ustring> Df::EncodingMenu::get_text() const {
  const auto i = combobox->get_active_row_number();
  if (0 <= i) {
    return encodings.at(i);
  }

  return boost::none;
}

/*!
 * \brief Map an encoding name to its standard form
 */
boost::optional<Glib::ustring>
Df::EncodingMenu::norm_encoding(const boost::optional<Glib::ustring> &e) {
  if (!e) {
    return boost::none;
  }

  Glib::ustring res{boost::get(e)};
  for (auto i = 0u; i < res.size(); ++i) {
    if ('-' == res[i]) {
      res.replace(i, 1, "_");
    }
  }

  return res.lowercase();
}

void Df::EncodingMenu::set_text(const Glib::ustring encoding) {
  const auto tmp_enc = norm_encoding(encoding);
  auto cit = std::find(encodings.cbegin(), encodings.cend(), tmp_enc);
  if (cit != encodings.cend()) {
    combobox->set_active(std::distance(encodings.cbegin(), cit));
  }
}

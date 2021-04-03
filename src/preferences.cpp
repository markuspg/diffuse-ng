/*
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2021      Markus Prasser <markuspg@gmx.de>
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

#include "preferences.h"

namespace Df = Diffuse;

std::optional<Glib::ustring>
Df::Preferences::convertToNativePath(const Glib::ustring &s) {
  return std::optional<Glib::ustring>{};
}

/*!
 * \brief Get method to retrieve the preference value
 * \param[in] name The name of the preferences whose value shall be retrieved
 * \return The preference's value
 * \throws std::out_of_range If there is no preference with name stored
 */
bool Df::Preferences::getBool(const Glib::ustring &name) {
  return bool_prefs.at(name);
}

/*!
 * \brief Get method to retrieve the preference value
 * \param[in] name The name of the preferences whose value shall be retrieved
 * \return The preference's value
 * \throws std::out_of_range If there is no preference with name stored
 */
int Df::Preferences::getInt(const Glib::ustring &name) {
  return int_prefs.at(name);
}

/*!
 * \brief Get method to retrieve the preference value
 * \param[in] name The name of the preferences whose value shall be retrieved
 * \return The preference's value
 * \throws std::out_of_range If there is no preference with name stored
 */
Glib::ustring Df::Preferences::getString(const Glib::ustring &name) {
  return string_prefs.at(name);
}

/*!
 * \brief Set method to manipulate the preference value
 * \param[in] name The name of the preferences whose value shall be manipulated
 * \param[in] value The preference's new value
 */
void Df::Preferences::setBool(const Glib::ustring &name, bool value) {
  bool_prefs[name] = value;
}

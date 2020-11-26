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

#include "diffuse_utils.h"
#include "diffuse_globals.h"

#include <glibmm/convert.h>

#include <iostream>

namespace Df = Diffuse;

/*!
 * \brief Platform test
 */
bool Df::isWindows() {
  return false; // TODO: os.name == 'nt'
}

/*!
 * \brief Convenience function to display debug messages
 */
void Df::logDebug(const Glib::ustring &s) {
    std::cerr << APP_NAME + ": " + s << "\n";
}

/*!
 * \brief Print a UTF-8 string using the host's native encoding
 */
void Df::printMessage(const Glib::ustring &s) {
  try {
    std::cout << Glib::locale_from_utf8(s) << "\n";
  } catch (const Glib::ConvertError &) {
  }
}

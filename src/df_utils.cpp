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

#include "df_utils.h"
#include "df_globals.h"

#include <glibmm/convert.h>

#include <iostream>

namespace Df = Diffuse;

bool Df::isWindows() { return false; } // TODO

/**
 * @brief Convenience function to print debug messages to stderr
 * @param[in] s The debug message to be printed
 */
void Df::logDebug(const Glib::ustring &s) {
  // std::cerr << Glib::locale_from_utf8(
  //     Glib::ustring::compose("%1: %2\n", Df::APP_NAME, s));
}

/**
 * @brief Print a UTF-8 encoded string to stdout in the host's native encoding
 * @param[in] s The string which shall be printed
 */
void Df::printMessage(const Glib::ustring &s) {
  try {
    std::cout << Glib::locale_from_utf8(s) << "\n";
  } catch (const Glib::ConvertError &) {
  }
}

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

#include "df_utils.h"
#include "df_globals.h"
#include "df_message_dialog.h"

#include <glibmm/convert.h>

#include <iostream>

namespace Df = Diffuse;

bool Df::isWindows() { return false; }

/**
 * @brief Convenience function to write debug messages to stderr
 * @param[in] s The error message to display to the user
 */
void Df::logDebug(const Glib::ustring &s) {
  // std::cerr << Glib::locale_from_utf8(Glib::ustring{Df::APP_NAME} + ": " + s
  //                                     "\n");
}

/**
 * @brief Display an error message to the user
 * @param[in] s The error message to display to the user
 */
void Df::logError(const Glib::ustring &s) {
  MessageDialog m{Gtk::MESSAGE_ERROR, s};
  m.run();
}

/**
 * @brief Map an encoding name to its standard form
 * @param[in] e The encoding which shall be mapped
 * @return A mapped copy of the passed encoding
 */
std::optional<Glib::ustring>
Df::norm_encoding(const std::optional<Glib::ustring> &e) {
  std::optional<Glib::ustring> ret;

  if (e) {
    // e.replace('-', '_').lower() TODO
  }

  return ret;
}

/**
 * @brief Print a UTF-8 string using the host's native encoding
 * @param[in] s The message which shall be printed
 */
void Df::printMessage(const Glib::ustring &s) {
  try {
    std::cout << Glib::locale_from_utf8(s) << std::endl;
  } catch (const Glib::ConvertError &e) {
  }
}

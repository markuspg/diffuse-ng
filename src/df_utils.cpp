/* Diffuse - a graphical tool for merging and comparing text files
 * Copyright (C) 2022 Markus Prasser
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "df_utils.h"
#include "df_message_dialog.h"

#include <glibmm/convert.h>
#include <glibmm/miscutils.h>

#include <iostream>

namespace Df = Diffuse;

/**
 * @brief Report error messages by means of a user-visible message dialog
 * @param[in] s The error message to display to the user
 */
void Df::logError(const Glib::ustring &s) {
  MessageDialog m{Gtk::MESSAGE_ERROR, s};
  m.run();
}

/**
 * @brief Create nested subdirectories within a particular directory
 * @param[in,out] p The directory in which the subdirectories shall be created -
 * in case of success this will be set to the new path
 * @param[in] ss A list of subdirectories in the order they shall be nested
 * (outer to inner)
 * @return _true_ on success (or if the subdirectories already exist) or _false_
 * otherwise
 */
bool Df::make_subdirs(Glib::ustring &p, const std::vector<Glib::ustring> &ss) {
  std::vector<Glib::ustring> tmp_vec{ss};
  tmp_vec.emplace(tmp_vec.cbegin(), p);

  std::string tmp_path{Glib::build_filename(tmp_vec)};
  if (0 == g_mkdir_with_parents(tmp_path.c_str(), 0700)) {
    p = tmp_path;
    return true;
  }

  return false;
}

/**
 * @brief Print a UTF-8 encoded string using the host's native encoding
 * @param[in] s A UTF-8 encoded string which shall be printed
 */
void Df::printMessage(const Glib::ustring &s) {
  try {
    std::cout << Glib::locale_from_utf8(s);
  } catch (const Glib::ConvertError &e) {
  }
}

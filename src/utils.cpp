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

#include "utils.h"

#include <glibmm/convert.h>
#include <glibmm/miscutils.h>

#include <iostream>

namespace Df = Diffuse;

/*!
 * \brief Platform test
 */
bool Df::isWindows() { return false; }

void Df::logError(const Glib::ustring &s) {}

/*!
 * \brief Create nested subdirectories and return the complete path
 * \param[in,out] p The path within which the subdirectories shall be created.
 * This will contain the resulting full path on success
 * \param[in] ss The names of the subdirectory hierarchy which shall be created
 * \return True on success, false otherwise
 */
bool Df::make_subdirs(Glib::ustring &p, const std::vector<Glib::ustring> &ss) {
  std::vector<Glib::ustring> path_components{ss};
  path_components.emplace(path_components.cbegin(), p);
  Glib::ustring path{Glib::build_path(G_DIR_SEPARATOR_S, path_components)};

  if (0 == g_mkdir_with_parents(path.c_str(), 0755)) {
    p = path;
    return true;
  }

  return false;
}

/*!
 * \brief Print a UTF-8 string using the host's native encoding
 * \param[in] s The UTF-8 encoded string that shall be printed
 */
void Df::printMessage(const Glib::ustring &s) {
  try {
    std::cout << Glib::locale_from_utf8(s) << "\n";
  } catch (const Glib::ConvertError &) {
  }
}

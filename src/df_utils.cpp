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
#include "df_message_dialog.h"

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
 * @brief Report error messages to the user through a message dialog window
 * @param[in] s The error message to display to the user
 */
void Df::logError(const Glib::ustring &s) {
  MessageDialog m{Gtk::MessageType::MESSAGE_ERROR, s};
  m.run();
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

std::optional<std::vector<Glib::ustring>>
Df::readconfiglines(std::ifstream &fd) {
  if (fd.fail()) {
    std::cerr << "File input stream is in failed state\n";
    return std::nullopt;
  }

  std::ostringstream sstr;
  sstr << fd.rdbuf();
  auto file_content{Glib::locale_to_utf8(sstr.str())};

  if (fd.fail()) {
    std::cerr << "File input stream is in failed state after reading\n";
    return std::nullopt;
  }

  const auto cr_regex{Glib::Regex::create("\\r")};
  file_content = cr_regex->replace(file_content, 0, "",
                                   static_cast<Glib::RegexMatchFlags>(0));

  const auto lf_regex{Glib::Regex::create("\\n")};
  return lf_regex->split(file_content);
}

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
#include "diffuse_messagedialog.h"

#include <glib.h>
#include <glibmm/convert.h>
#include <glibmm/miscutils.h>

#include <iostream>
#include <limits>

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
 * \brief Report error messages
 */
void Df::logError(const Glib::ustring &s) {
  MessageDialog m{Gtk::MESSAGE_ERROR, s};
  m.run();
  // m.destroy()
}

/*!
 * \brief Create nested subdirectories and return the complete path
 *
 * On success the new directory's path is returned in the first argument
 */
bool Df::make_subdirs(std::string &p, const std::vector<Glib::ustring> &ss) {
  std::vector<Glib::ustring> path_components;
  path_components.emplace_back(p);
  for (const auto &s : ss) {
    path_components.emplace_back(s);
  }
  const auto full_path = Glib::build_path(G_DIR_SEPARATOR_S, path_components);
  if (0 == g_mkdir_with_parents(full_path.c_str(), 0755)) {
    p = full_path;
    return true;
  }

  return false;
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

boost::optional<std::pair<Glib::ustring, Glib::ustring>>
Df::run(const std::vector<Glib::ustring> &cmdline) {
  std::vector<gchar *> argv(cmdline.size() + 1, nullptr);
  auto i = 0u;
  for (const auto &elem : cmdline) {
    argv[i++] = g_strdup(elem.data());
  }
  argv[i] = nullptr;

  GError *er = nullptr;
  gchar *err = nullptr;
  int exit_status = std::numeric_limits<int>::max();
  gchar *out = nullptr;
  const bool res =
      g_spawn_sync(nullptr, argv.data(), nullptr, G_SPAWN_DEFAULT, nullptr,
                   nullptr, &out, &err, &exit_status, &er);
  Glib::ustring err_str{err ? err : ""};
  g_free(err);
  Glib::ustring out_str{out ? out : ""};
  g_free(out);
  for (const auto &ptr : argv) {
    g_free(ptr);
  }
  if (!res) {
    return boost::none;
  }

  if (false == g_spawn_check_exit_status(exit_status, &er)) {
    return boost::none;
  }

  return {std::make_pair<Glib::ustring, Glib::ustring>(std::move(err_str),
                                                       std::move(out_str))};
}

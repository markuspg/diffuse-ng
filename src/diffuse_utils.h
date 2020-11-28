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

#ifndef DIFFUSE_UTILS
#define DIFFUSE_UTILS

#include <glibmm/ustring.h>

#include <vector>

#include <boost/optional.hpp>

namespace Diffuse {
bool isWindows();
void logDebug(const Glib::ustring &s);
void logError(const Glib::ustring &s);
bool make_subdirs(std::string &p, const std::vector<Glib::ustring> &ss);
void printMessage(const Glib::ustring &s);
boost::optional<std::vector<Glib::ustring>>
read_lines_utf8(const Glib::ustring &path);
boost::optional<std::pair<Glib::ustring, Glib::ustring>>
run(const std::vector<Glib::ustring> &cmdline);
} // namespace Diffuse

#endif // DIFFUSE_UTILS

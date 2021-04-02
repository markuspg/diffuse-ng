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

#ifndef DIFFUSE_PREFERENCES_H_
#define DIFFUSE_PREFERENCES_H_

#include <glibmm/ustring.h>

#include <optional>

namespace Diffuse {
class Preferences {
public:
  std::optional<Glib::ustring> convertToNativePath(const Glib::ustring &s);
  void setBool(const Glib::ustring &name, bool value);
};
} // namespace Diffuse

#endif // DIFFUSE_PREFERENCES_H_

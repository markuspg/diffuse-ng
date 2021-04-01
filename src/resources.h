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

#ifndef DIFFUSE_RESOURCES_H_
#define DIFFUSE_RESOURCES_H_

#include "colour.h"

#include <glibmm/ustring.h>

#include <map>
#include <set>

namespace Diffuse {
class Resources {
public:
  Resources();

  bool parse(const Glib::ustring &file_name);

private:
  void setKeyBinding(const Glib::ustring &ctx, const Glib::ustring &s,
                     const Glib::ustring &v);
  void setDifferenceColours(const Glib::ustring &s);

  // Default colours
  const std::map<Glib::ustring, Colour> colours;

  // Default floats
  const std::map<Glib::ustring, float> floats;

  // Default keybindings
  const std::map<void *, void *> keybindings;
  const std::map<void *, void *> keybindings_lookup;

  // List of imported resource files (each file is imported once only)
  const std::set<void *> resource_files;

  // Default strings
  const std::map<Glib::ustring, Glib::ustring> strings;

  // Syntax highlighting support
  void *const current_syntax = nullptr;
  const std::map<void *, void *> syntax_file_patterns;
  const std::map<void *, void *> syntax_magic_patterns;
  const std::map<void *, void *> syntaxes;
};
} // namespace Diffuse

#endif // DIFFUSE_RESOURCES_H_

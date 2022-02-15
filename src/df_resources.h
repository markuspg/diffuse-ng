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

#ifndef DF_RESOURCES_H
#define DF_RESOURCES_H

#include "df_colour.h"

#include <glibmm/ustring.h>

#include <map>
#include <optional>
#include <set>
#include <vector>

namespace Diffuse {
class Resources {
public:
  Resources();

  Colour getColour(const Glib::ustring symbol);
  Colour getDifferenceColour(unsigned int i);
  float getFloat(const Glib::ustring symbol);
  Glib::ustring getString(const Glib::ustring symbol);
  bool parse(const std::string &file_name);

private:
  void set_binding(const Glib::ustring &ctx, const Glib::ustring &s,
                   const Glib::ustring &v);
  void setDifferenceColours(const Glib::ustring &s);

  // Default colours
  std::map<Glib::ustring, Colour> colours;
  std::vector<Glib::ustring> difference_colours;

  // Default floats
  std::map<Glib::ustring, float> floats;

  // Default strings
  std::map<Glib::ustring, Glib::ustring> strings;

  // Syntax highlighting support
  std::optional<void *> current_syntax;
  std::map<void *, void *> syntaxes;
  std::map<void *, void *> syntax_file_patterns;
  std::map<void *, void *> syntax_magic_patterns;

  // Default keybindings
  std::map<void *, void *> keybindings;
  std::map<void *, void *> keybindings_lookup;

  // List of imported resource files (each is being imported only once)
  std::set<void *> resource_files;
};
} // namespace Diffuse

#endif // DF_RESOURCES_H

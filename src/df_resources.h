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

#ifndef DF_RESOURCES_H
#define DF_RESOURCES_H

#include "df_colour.h"
#include "df_syntax_parser.h"

#include <glibmm/regex.h>
#include <glibmm/ustring.h>

#include <map>
#include <memory>
#include <set>
#include <vector>

namespace Diffuse {
/**
 * @brief Class for customizable behaviour not exposed by the preferences dialog
 *
 * This concerns attributes like hotkey assignments, colours, syntax
 * highlighting, etc.
 *
 * Syntax highlighting is implemented by '*.syntax' files which are normally
 * being read for the system-wide initialization file '/etc/diffuserc'. A
 * personal initialization file '~/diffuse/diffuserc' can be use to change the
 * default behaviour.
 */
class Resources {
public:
  Resources();

  Colour getColour(const Glib::ustring &symbol);
  Colour getDifferenceColour(std::vector<Colour>::size_type i);
  float getFloat(const Glib::ustring &symbol);
  Glib::ustring getString(const Glib::ustring &symbol);
  bool parse(const std::string &file_name);

private:
  void setDifferenceColours(const Glib::ustring &s);
  void setKeyBinding(const Glib::ustring &ctx, const Glib::ustring &s,
                     const Glib::ustring &v);

  std::map<Glib::ustring, Colour> colours;
  //! Colours used for indicating differences
  std::vector<Glib::ustring> difference_colours;
  std::map<Glib::ustring, float> floats;
  std::map<void *, void *> keybindings;
  std::map<void *, void *> keybindings_lookup;
  //! List of imported resource files (each should be imported once only)
  std::set<std::string> resource_files;
  std::map<Glib::ustring, Glib::ustring> strings;
  // Syntax highlighting support
  std::shared_ptr<SyntaxParser> current_syntax;
  std::map<Glib::ustring, std::shared_ptr<SyntaxParser>> syntaxes;
  std::map<Glib::ustring, Glib::RefPtr<Glib::Regex>> syntax_file_patterns;
  std::map<Glib::ustring, Glib::RefPtr<Glib::Regex>> syntax_magic_patterns;
};
} // namespace Diffuse

#endif // DF_RESOURCES_H

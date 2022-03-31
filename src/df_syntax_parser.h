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

#ifndef DF_SYNTAX_PARSER_H
#define DF_SYNTAX_PARSER_H

#include <glibmm/regex.h>
#include <glibmm/ustring.h>

namespace Diffuse {
/**
 * @brief Class to build and run a finite state machine for identifying syntax
 *   tokens
 */
class SyntaxParser {
public:
  /**
   * @brief Struct representing a transition between states
   */
  struct Transition {
    //! The pattern which initiates the transition when being matched
    const Glib::RefPtr<Glib::Regex> pattern;
    //! The classification of the matched characters
    const Glib::ustring token_type;
    //! The state machine's new state
    const Glib::ustring next_state;
  };
  using Transitions = std::vector<Transition>;

  SyntaxParser(const Glib::ustring &initial_state,
               const Glib::ustring &default_token_type);

  void addPattern(const Glib::ustring &prev_state,
                  const Glib::ustring &next_state,
                  const Glib::ustring &token_type,
                  const Glib::RefPtr<Glib::Regex> &pattern);

private:
  //! Default classification of characters that are not explicitly matched by
  //! any state transition patterns
  const Glib::ustring default_token_type;
  //! Initial state for the state machine when parsing a new file
  const Glib::ustring initial_state;
  //! Mappings from a state to a list of Transition object indicating the new
  //! state for the state machine when "pattern" is matched and how to classify
  //! the matched characters
  std::map<Glib::ustring, Transitions> transitions_lookup;
};
} // namespace Diffuse

#endif // DF_SYNTAX_PARSER_H

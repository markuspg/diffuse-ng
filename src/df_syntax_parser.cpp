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

#include "df_syntax_parser.h"

namespace Df = Diffuse;

/**
 * @brief Create a new state machine that begins in initial_state and
 *   classifies all characters not matched by the patterns as
 *   default_token_type
 * @param[in] initial_state The name of the initial state the state machine
 *   shall be in
 * @param[in] default_token_type The token type unmatched characters shall be
 *   of
 */
Df::SyntaxParser::SyntaxParser(const Glib::ustring &init_state,
                               const Glib::ustring &dflt_token_type)
    : default_token_type{dflt_token_type}, initial_state{init_state},
      transitions_lookup{{init_state, {}}} {}

/**
 * @brief Add new edge to the finite state machine from prev_state to next_state
 *
 * Characters will be identified as token_type when pattern is matched. Any
 * newly referenced state will be added. Patterns for edges leaving a state will
 * be tested in the order they were added to the finite state machine.
 *
 * @param prev_state
 * @param next_state
 * @param token_type
 * @param pattern
 */
void Df::SyntaxParser::addPattern(const Glib::ustring &prev_state,
                                  const Glib::ustring &next_state,
                                  const Glib::ustring &token_type,
                                  const Glib::RefPtr<Glib::Regex> &pattern) {
  for (const auto &state :
       std::array<Glib::ustring, 2>{prev_state, next_state}) {
    if (transitions_lookup.cend() == transitions_lookup.find(state)) {
      transitions_lookup.emplace(state, Transitions{});
    }
  }
  transitions_lookup[prev_state].emplace_back(
      Transition{pattern, token_type, next_state});
}

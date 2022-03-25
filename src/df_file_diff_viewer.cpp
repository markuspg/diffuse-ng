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

#include "df_file_diff_viewer.h"

namespace Df = Diffuse;

Df::FileDiffViewer::FileDiffViewer(guint n, Preferences &prfs)
    : Gtk::Table{3, n + 1},
      line_mode_actions{
          {"enter_align_mode", &FileDiffViewer::line_mode_enter_align_mode},
          {"enter_character_mode", &FileDiffViewer::setCharMode},
          {"first_line", &FileDiffViewer::first_line},
          {"extend_first_line", &FileDiffViewer::extend_first_line},
          {"last_line", &FileDiffViewer::last_line},
          {"extend_last_line", &FileDiffViewer::extend_last_line},
          // {"up", &FileDiffViewer::line_mode_up},
          {"extend_up", &FileDiffViewer::line_mode_extend_up}},
      prefs{prfs} {
  // Verify that there is a valid number of panes
  if (2 > n) {
    throw std::runtime_error{"Invalid number of panes"};
  }

  set_flags(Gtk::WidgetFlags::CAN_FOCUS);
}

/**
 * @brief "extend_first_line" keybinding action
 */
void Df::FileDiffViewer::extend_first_line() {}

/**
 * @brief "extend_last_line" keybinding action
 */
void Df::FileDiffViewer::extend_last_line() {}

/**
 * @brief "extend_up" keybinding action
 */
void Df::FileDiffViewer::line_mode_extend_up() {}

/**
 * @brief "first_line" keybinding action
 */
void Df::FileDiffViewer::first_line() {}

/**
 * @brief Returns True if there are any differences
 * @return _true_ if there are any differences, _false_ otherwise
 */
bool Df::FileDiffViewer::hasDifferences() const { return false; }

/**
 * @brief "last_line" keybinding action
 */
void Df::FileDiffViewer::last_line(){};

/**
 * @brief "enter_align_mode" keybinding action
 */
void Df::FileDiffViewer::line_mode_enter_align_mode() {}

/**
 * @brief "up" keybinding action
 * @param selection
 */
void Df::FileDiffViewer::line_mode_up(void *selection) {}

/**
 * @brief Changes the viewer's mode to CHAR_MODE
 */
void Df::FileDiffViewer::setCharMode() {}

Df::FileDiffViewer::Line::Line(std::optional<unsigned> &line_no,
                               std::optional<Glib::ustring> &txt)
    : line_number{line_no}, text{txt} {}

std::optional<Glib::ustring> Df::FileDiffViewer::Line::getText() const {
  if (is_modified) {
    return modified_text;
  }

  return text;
}

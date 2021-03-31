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

#include "resources.h"

namespace Df = Diffuse;

Df::Resources::Resources() {
  // Default keybindings
  setKeyBinding("menu", "open_file", "Ctrl+o");
  setKeyBinding("menu", "open_file_in_new_tab", "Ctrl+t");
  setKeyBinding("menu", "open_modified_files", "Shift+Ctrl+O");
  setKeyBinding("menu", "open_commit", "Shift+Ctrl+T");
  setKeyBinding("menu", "reload_file", "Shift+Ctrl+R");
  setKeyBinding("menu", "save_file", "Ctrl+s");
  setKeyBinding("menu", "save_file_as", "Shift+Ctrl+A");
  setKeyBinding("menu", "save_all", "Shift+Ctrl+S");
  setKeyBinding("menu", "new_2_way_file_merge", "Ctrl+2");
  setKeyBinding("menu", "new_3_way_file_merge", "Ctrl+3");
  setKeyBinding("menu", "new_n_way_file_merge", "Ctrl+4");
  setKeyBinding("menu", "close_tab", "Ctrl+w");
  setKeyBinding("menu", "undo_close_tab", "Shift+Ctrl+W");
  setKeyBinding("menu", "quit", "Ctrl+q");
  setKeyBinding("menu", "undo", "Ctrl+z");
  setKeyBinding("menu", "redo", "Shift+Ctrl+Z");
  setKeyBinding("menu", "cut", "Ctrl+x");
  setKeyBinding("menu", "copy", "Ctrl+c");
  setKeyBinding("menu", "paste", "Ctrl+v");
  setKeyBinding("menu", "select_all", "Ctrl+a");
  setKeyBinding("menu", "clear_edits", "Ctrl+r");
  setKeyBinding("menu", "dismiss_all_edits", "Ctrl+d");
  setKeyBinding("menu", "find", "Ctrl+f");
  setKeyBinding("menu", "find_next", "Ctrl+g");
  setKeyBinding("menu", "find_previous", "Shift+Ctrl+G");
  setKeyBinding("menu", "go_to_line", "Shift+Ctrl+L");
  setKeyBinding("menu", "realign_all", "Ctrl+l");
  setKeyBinding("menu", "isolate", "Ctrl+i");
  setKeyBinding("menu", "first_difference", "Shift+Ctrl+Up");
  setKeyBinding("menu", "previous_difference", "Ctrl+Up");
  setKeyBinding("menu", "next_difference", "Ctrl+Down");
  setKeyBinding("menu", "last_difference", "Shift+Ctrl+Down");
  setKeyBinding("menu", "first_tab", "Shift+Ctrl+Page_Up");
  setKeyBinding("menu", "previous_tab", "Ctrl+Page_Up");
  setKeyBinding("menu", "next_tab", "Ctrl+Page_Down");
  setKeyBinding("menu", "last_tab", "Shift+Ctrl+Page_Down");
  setKeyBinding("menu", "shift_pane_right", "Shift+Ctrl+parenright");
  setKeyBinding("menu", "shift_pane_left", "Shift+Ctrl+parenleft");
  setKeyBinding("menu", "convert_to_upper_case", "Ctrl+u");
  setKeyBinding("menu", "convert_to_lower_case", "Shift+Ctrl+U");
  setKeyBinding("menu", "sort_lines_in_ascending_order", "Ctrl+y");
  setKeyBinding("menu", "sort_lines_in_descending_order", "Shift+Ctrl+Y");
  setKeyBinding("menu", "remove_trailing_white_space", "Ctrl+k");
  setKeyBinding("menu", "convert_tabs_to_spaces", "Ctrl+b");
  setKeyBinding("menu", "convert_leading_spaces_to_tabs", "Shift+Ctrl+B");
  setKeyBinding("menu", "increase_indenting", "Shift+Ctrl+greater");
  setKeyBinding("menu", "decrease_indenting", "Shift+Ctrl+less");
  setKeyBinding("menu", "convert_to_dos", "Shift+Ctrl+E");
  setKeyBinding("menu", "convert_to_mac", "Shift+Ctrl+C");
  setKeyBinding("menu", "convert_to_unix", "Ctrl+e");
  setKeyBinding("menu", "copy_selection_right", "Shift+Ctrl+Right");
  setKeyBinding("menu", "copy_selection_left", "Shift+Ctrl+Left");
  setKeyBinding("menu", "copy_left_into_selection", "Ctrl+Right");
  setKeyBinding("menu", "copy_right_into_selection", "Ctrl+Left");
  setKeyBinding("menu", "merge_from_left_then_right", "Ctrl+m");
  setKeyBinding("menu", "merge_from_right_then_left", "Shift+Ctrl+M");
  setKeyBinding("menu", "help_contents", "F1");
  setKeyBinding("line_mode", "enter_align_mode", "space");
  setKeyBinding("line_mode", "enter_character_mode", "Return");
  setKeyBinding("line_mode", "enter_character_mode", "KP_Enter");
  setKeyBinding("line_mode", "first_line", "Home");
  setKeyBinding("line_mode", "first_line", "g");
  setKeyBinding("line_mode", "extend_first_line", "Shift+Home");
  setKeyBinding("line_mode", "last_line", "End");
  setKeyBinding("line_mode", "last_line", "Shift+G");
  setKeyBinding("line_mode", "extend_last_line", "Shift+End");
  setKeyBinding("line_mode", "up", "Up");
  setKeyBinding("line_mode", "up", "k");
  setKeyBinding("line_mode", "extend_up", "Shift+Up");
  setKeyBinding("line_mode", "extend_up", "Shift+K");
  setKeyBinding("line_mode", "down", "Down");
  setKeyBinding("line_mode", "down", "j");
  setKeyBinding("line_mode", "extend_down", "Shift+Down");
  setKeyBinding("line_mode", "extend_down", "Shift+J");
  setKeyBinding("line_mode", "left", "Left");
  setKeyBinding("line_mode", "left", "h");
  setKeyBinding("line_mode", "extend_left", "Shift+Left");
  setKeyBinding("line_mode", "right", "Right");
  setKeyBinding("line_mode", "right", "l");
  setKeyBinding("line_mode", "extend_right", "Shift+Right");
  setKeyBinding("line_mode", "page_up", "Page_Up");
  setKeyBinding("line_mode", "page_up", "Ctrl+u");
  setKeyBinding("line_mode", "extend_page_up", "Shift+Page_Up");
  setKeyBinding("line_mode", "extend_page_up", "Shift+Ctrl+U");
  setKeyBinding("line_mode", "page_down", "Page_Down");
  setKeyBinding("line_mode", "page_down", "Ctrl+d");
  setKeyBinding("line_mode", "extend_page_down", "Shift+Page_Down");
  setKeyBinding("line_mode", "extend_page_down", "Shift+Ctrl+D");
  setKeyBinding("line_mode", "delete_text", "BackSpace");
  setKeyBinding("line_mode", "delete_text", "Delete");
  setKeyBinding("line_mode", "delete_text", "x");
  setKeyBinding("line_mode", "clear_edits", "r");
  setKeyBinding("line_mode", "isolate", "i");
  setKeyBinding("line_mode", "first_difference", "Ctrl+Home");
  setKeyBinding("line_mode", "first_difference", "Shift+P");
  setKeyBinding("line_mode", "previous_difference", "p");
  setKeyBinding("line_mode", "next_difference", "n");
  setKeyBinding("line_mode", "last_difference", "Ctrl+End");
  setKeyBinding("line_mode", "last_difference", "Shift+N");
  // setKeyBinding("line_mode", "copy_selection_right", "Shift+L");
  // setKeyBinding("line_mode", "copy_selection_left", "Shift+H");
  setKeyBinding("line_mode", "copy_left_into_selection", "Shift+L");
  setKeyBinding("line_mode", "copy_right_into_selection", "Shift+H");
  setKeyBinding("line_mode", "merge_from_left_then_right", "m");
  setKeyBinding("line_mode", "merge_from_right_then_left", "Shift+M");
  setKeyBinding("align_mode", "enter_line_mode", "Escape");
  setKeyBinding("align_mode", "align", "space");
  setKeyBinding("align_mode", "enter_character_mode", "Return");
  setKeyBinding("align_mode", "enter_character_mode", "KP_Enter");
  setKeyBinding("align_mode", "first_line", "g");
  setKeyBinding("align_mode", "last_line", "Shift+G");
  setKeyBinding("align_mode", "up", "Up");
  setKeyBinding("align_mode", "up", "k");
  setKeyBinding("align_mode", "down", "Down");
  setKeyBinding("align_mode", "down", "j");
  setKeyBinding("align_mode", "left", "Left");
  setKeyBinding("align_mode", "left", "h");
  setKeyBinding("align_mode", "right", "Right");
  setKeyBinding("align_mode", "right", "l");
  setKeyBinding("align_mode", "page_up", "Page_Up");
  setKeyBinding("align_mode", "page_up", "Ctrl+u");
  setKeyBinding("align_mode", "page_down", "Page_Down");
  setKeyBinding("align_mode", "page_down", "Ctrl+d");
  setKeyBinding("character_mode", "enter_line_mode", "Escape");
}

/*!
 * \brief Keyboard action processing
 * \param[in] ctx
 * \param[in] s
 * \param[in] v
 */
void Df::Resources::setKeyBinding(const Glib::ustring &ctx,
                                  const Glib::ustring &s,
                                  const Glib::ustring &v) {}

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

#include "df_resources.h"

namespace Df = Diffuse;

Df::Resources::Resources() {
  set_binding("menu", "open_file", "Ctrl+o");
  set_binding("menu", "open_file_in_new_tab", "Ctrl+t");
  set_binding("menu", "open_modified_files", "Shift+Ctrl+O");
  set_binding("menu", "open_commit", "Shift+Ctrl+T");
  set_binding("menu", "reload_file", "Shift+Ctrl+R");
  set_binding("menu", "save_file", "Ctrl+s");
  set_binding("menu", "save_file_as", "Shift+Ctrl+A");
  set_binding("menu", "save_all", "Shift+Ctrl+S");
  set_binding("menu", "new_2_way_file_merge", "Ctrl+2");
  set_binding("menu", "new_3_way_file_merge", "Ctrl+3");
  set_binding("menu", "new_n_way_file_merge", "Ctrl+4");
  set_binding("menu", "close_tab", "Ctrl+w");
  set_binding("menu", "undo_close_tab", "Shift+Ctrl+W");
  set_binding("menu", "quit", "Ctrl+q");
  set_binding("menu", "undo", "Ctrl+z");
  set_binding("menu", "redo", "Shift+Ctrl+Z");
  set_binding("menu", "cut", "Ctrl+x");
  set_binding("menu", "copy", "Ctrl+c");
  set_binding("menu", "paste", "Ctrl+v");
  set_binding("menu", "select_all", "Ctrl+a");
  set_binding("menu", "clear_edits", "Ctrl+r");
  set_binding("menu", "dismiss_all_edits", "Ctrl+d");
  set_binding("menu", "find", "Ctrl+f");
  set_binding("menu", "find_next", "Ctrl+g");
  set_binding("menu", "find_previous", "Shift+Ctrl+G");
  set_binding("menu", "go_to_line", "Shift+Ctrl+L");
  set_binding("menu", "realign_all", "Ctrl+l");
  set_binding("menu", "isolate", "Ctrl+i");
  set_binding("menu", "first_difference", "Shift+Ctrl+Up");
  set_binding("menu", "previous_difference", "Ctrl+Up");
  set_binding("menu", "next_difference", "Ctrl+Down");
  set_binding("menu", "last_difference", "Shift+Ctrl+Down");
  set_binding("menu", "first_tab", "Shift+Ctrl+Page_Up");
  set_binding("menu", "previous_tab", "Ctrl+Page_Up");
  set_binding("menu", "next_tab", "Ctrl+Page_Down");
  set_binding("menu", "last_tab", "Shift+Ctrl+Page_Down");
  set_binding("menu", "shift_pane_right", "Shift+Ctrl+parenright");
  set_binding("menu", "shift_pane_left", "Shift+Ctrl+parenleft");
  set_binding("menu", "convert_to_upper_case", "Ctrl+u");
  set_binding("menu", "convert_to_lower_case", "Shift+Ctrl+U");
  set_binding("menu", "sort_lines_in_ascending_order", "Ctrl+y");
  set_binding("menu", "sort_lines_in_descending_order", "Shift+Ctrl+Y");
  set_binding("menu", "remove_trailing_white_space", "Ctrl+k");
  set_binding("menu", "convert_tabs_to_spaces", "Ctrl+b");
  set_binding("menu", "convert_leading_spaces_to_tabs", "Shift+Ctrl+B");
  set_binding("menu", "increase_indenting", "Shift+Ctrl+greater");
  set_binding("menu", "decrease_indenting", "Shift+Ctrl+less");
  set_binding("menu", "convert_to_dos", "Shift+Ctrl+E");
  set_binding("menu", "convert_to_mac", "Shift+Ctrl+C");
  set_binding("menu", "convert_to_unix", "Ctrl+e");
  set_binding("menu", "copy_selection_right", "Shift+Ctrl+Right");
  set_binding("menu", "copy_selection_left", "Shift+Ctrl+Left");
  set_binding("menu", "copy_left_into_selection", "Ctrl+Right");
  set_binding("menu", "copy_right_into_selection", "Ctrl+Left");
  set_binding("menu", "merge_from_left_then_right", "Ctrl+m");
  set_binding("menu", "merge_from_right_then_left", "Shift+Ctrl+M");
  set_binding("menu", "help_contents", "F1");
  set_binding("line_mode", "enter_align_mode", "space");
  set_binding("line_mode", "enter_character_mode", "Return");
  set_binding("line_mode", "enter_character_mode", "KP_Enter");
  set_binding("line_mode", "first_line", "Home");
  set_binding("line_mode", "first_line", "g");
  set_binding("line_mode", "extend_first_line", "Shift+Home");
  set_binding("line_mode", "last_line", "End");
  set_binding("line_mode", "last_line", "Shift+G");
  set_binding("line_mode", "extend_last_line", "Shift+End");
  set_binding("line_mode", "up", "Up");
  set_binding("line_mode", "up", "k");
  set_binding("line_mode", "extend_up", "Shift+Up");
  set_binding("line_mode", "extend_up", "Shift+K");
  set_binding("line_mode", "down", "Down");
  set_binding("line_mode", "down", "j");
  set_binding("line_mode", "extend_down", "Shift+Down");
  set_binding("line_mode", "extend_down", "Shift+J");
  set_binding("line_mode", "left", "Left");
  set_binding("line_mode", "left", "h");
  set_binding("line_mode", "extend_left", "Shift+Left");
  set_binding("line_mode", "right", "Right");
  set_binding("line_mode", "right", "l");
  set_binding("line_mode", "extend_right", "Shift+Right");
  set_binding("line_mode", "page_up", "Page_Up");
  set_binding("line_mode", "page_up", "Ctrl+u");
  set_binding("line_mode", "extend_page_up", "Shift+Page_Up");
  set_binding("line_mode", "extend_page_up", "Shift+Ctrl+U");
  set_binding("line_mode", "page_down", "Page_Down");
  set_binding("line_mode", "page_down", "Ctrl+d");
  set_binding("line_mode", "extend_page_down", "Shift+Page_Down");
  set_binding("line_mode", "extend_page_down", "Shift+Ctrl+D");
  set_binding("line_mode", "delete_text", "BackSpace");
  set_binding("line_mode", "delete_text", "Delete");
  set_binding("line_mode", "delete_text", "x");
  set_binding("line_mode", "clear_edits", "r");
  set_binding("line_mode", "isolate", "i");
  set_binding("line_mode", "first_difference", "Ctrl+Home");
  set_binding("line_mode", "first_difference", "Shift+P");
  set_binding("line_mode", "previous_difference", "p");
  set_binding("line_mode", "next_difference", "n");
  set_binding("line_mode", "last_difference", "Ctrl+End");
  set_binding("line_mode", "last_difference", "Shift+N");
  // set_binding("line_mode", "copy_selection_right", "Shift+L");
  // set_binding("line_mode", "copy_selection_left", "Shift+H");
  set_binding("line_mode", "copy_left_into_selection", "Shift+L");
  set_binding("line_mode", "copy_right_into_selection", "Shift+H");
  set_binding("line_mode", "merge_from_left_then_right", "m");
  set_binding("line_mode", "merge_from_right_then_left", "Shift+M");
  set_binding("align_mode", "enter_line_mode", "Escape");
  set_binding("align_mode", "align", "space");
  set_binding("align_mode", "enter_character_mode", "Return");
  set_binding("align_mode", "enter_character_mode", "KP_Enter");
  set_binding("align_mode", "first_line", "g");
  set_binding("align_mode", "last_line", "Shift+G");
  set_binding("align_mode", "up", "Up");
  set_binding("align_mode", "up", "k");
  set_binding("align_mode", "down", "Down");
  set_binding("align_mode", "down", "j");
  set_binding("align_mode", "left", "Left");
  set_binding("align_mode", "left", "h");
  set_binding("align_mode", "right", "Right");
  set_binding("align_mode", "right", "l");
  set_binding("align_mode", "page_up", "Page_Up");
  set_binding("align_mode", "page_up", "Ctrl+u");
  set_binding("align_mode", "page_down", "Page_Down");
  set_binding("align_mode", "page_down", "Ctrl+d");
  set_binding("character_mode", "enter_line_mode", "Escape");
}

bool Df::Resources::parse(const std::string &file_name) { return true; }

void Df::Resources::set_binding(const Glib::ustring &ctx,
                                const Glib::ustring &s,
                                const Glib::ustring &v) {}

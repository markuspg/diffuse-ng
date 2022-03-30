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

#include "df_resources.h"
#include "df_utils.h"

#include <glibmm/regex.h>

#include <functional>
#include <iostream>

namespace Df = Diffuse;

Df::Resources::Resources()
    : colours{{"alignment", {1.0f, 1.0f, 0.0f}},
              {"character_selection", {0.7f, 0.7f, 1.0f}},
              {"cursor", {0.0f, 0.0f, 0.0f}},
              {"difference_1", {1.0f, 0.625f, 0.625f}},
              {"difference_2", {0.85f, 0.625f, 0.775f}},
              {"difference_3", {0.85f, 0.775f, 0.625f}},
              {"hatch", {0.8f, 0.8f, 0.8f}},
              {"line_number", {0.0f, 0.0f, 0.0f}},
              {"line_number_background", {0.75f, 0.75f, 0.75f}},
              {"line_selection", {0.7f, 0.7f, 1.0f}},
              {"map_background", {0.6f, 0.6f, 0.6f}},
              {"margin", {0.8f, 0.8f, 0.8f}},
              {"edited", {0.5f, 1.0f, 0.5f}},
              {"preedit", {0.0f, 0.0f, 0.0f}},
              {"text", {0.0f, 0.0f, 0.0f}},
              {"text_background", {1.0f, 1.0f, 1.0f}}},
      floats{{"alignment_opacity", 1.0f},
             {"character_difference_opacity", 0.4f},
             {"character_selection_opacity", 0.4f},
             {"edited_opacity", 0.4f},
             {"line_difference_opacity", 0.3f},
             {"line_selection_opacity", 0.4f}} {
  using namespace std::placeholders;

  auto set_binding =
      std::bind(&Df::Resources::setKeyBinding, *this, _1, _2, _3);
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

  Df::Resources::setDifferenceColours("difference_1 difference_2 difference_3");
}

Df::Colour Df::Resources::getColour(const Glib::ustring &symbol) {
  try {
    return colours.at(symbol);
  } catch (const std::out_of_range &) {
    logDebug(Glib::ustring::compose("Warning: unknown colour \"%1\"", symbol));
    return colours.emplace(symbol, Colour{0.0f, 0.0f, 0.0f}).first->second;
  }
}

Df::Colour
Df::Resources::getDifferenceColour(const std::vector<Colour>::size_type i) {
  const auto n = difference_colours.size();
  return getColour(difference_colours[(i + n - 1) % n]);
}

float Df::Resources::getFloat(const Glib::ustring &symbol) {
  try {
    return floats.at(symbol);
  } catch (const std::out_of_range &) {
    logDebug(Glib::ustring::compose("Warning: unknown float \"%1\"", symbol));
    return floats.emplace(symbol, 0.5f).first->second;
  }
}

Glib::ustring Df::Resources::getString(const Glib::ustring &symbol) {
  try {
    return strings.at(symbol);
  } catch (const std::out_of_range &) {
    logDebug(Glib::ustring::compose("Warning: unknown string \"%1\"", symbol));
    return strings.emplace(symbol, "").first->second;
  }
}

/**
 * @brief Parse resource files
 * @param[in] file_name The path of the file which shall be parsed
 * @return _true_ on success or if the file was parsed already, _false_
 *   otherwise
 */
bool Df::Resources::parse(const std::string &file_name) {
  // Only process files once
  if (resource_files.cend() != resource_files.find(file_name)) {
    return true;
  }
  // TODO: Wouldn't this be better at the end of this function?
  resource_files.emplace(file_name);

  std::ifstream f{file_name};
  if (f.fail()) {
    std::cerr << "Failed to open resource file\n";
    return false;
  }
  const auto tmp_res{readconfiglines(f)};
  f.close();
  if (f.fail()) {
    std::cerr << "Failed to close resource file\n";
    // A failure on closing the file should not fail the parsing
  }
  if (!tmp_res.has_value()) {
    std::cerr << "Failed to read resource file\n";
    return false;
  }

  const auto ss{tmp_res.value()};

  auto i = 0;
  // FIXME: improve validation
  for (const auto &s : ss) {
    ++i;

    const auto args{shlex_split(s, true)};
    if (args.empty()) {
      continue;
    }

    try {
      // e.g. add Python syntax highlighting:
      //   import /usr/share/diffuse/syntax/python.syntax
      if (("import" == args[0]) && (2 == args.size())) {
      }
      // e.g. make Ctrl+o trigger the open_file menu item
      //   keybinding menu open_file Ctrl+o
      else if (("keybinding" == args[0]) && (4 == args.size())) {
        setKeyBinding(args[1], args[2], args[3]);
      }
      // e.g. set the regular background colour to white
      //   colour text_background 1.0 1.0 1.0
      else if ((("color" == args[0]) || ("colour" == args[0])) &&
               (5 == args.size())) {
        colours.emplace(args[1], Colour{std::stof(args[2]), std::stof(args[3]),
                                        std::stof(args[4])});
      }
      // e.g. set opacity of the line_selection colour
      //   float line_selection_opacity 0.4
      else if (("float" == args[0]) && (3 == args.size())) {
        floats.emplace(args[1], std::stof(args[2]));
      }
      // e.g. set the help browser
      //   string help_browser gnome-help
      else if (("string" == args[0]) && (3 == args.size())) {
        strings.emplace(args[1], args[2]);
        if ("difference_colours" == args[1]) {
          setDifferenceColours(args[2]);
        }
      }
      // e.g. start a syntax specification for Python
      //   syntax Python normal text
      // where "normal" is the name of the default state and "text" is the
      // classification of all characters not explicitly matched by a syntax
      // highlighting rule
      else if (("syntax" == args[0]) &&
               ((2 == args.size()) || (4 == args.size()))) {
        const auto key{args[1]};
        if (2 == args.size()) {
          // Remove file pattern for a syntax specification
          syntax_file_patterns.erase(key);

          // Remove magic pattern for a syntax specification
          syntax_magic_patterns.erase(key);

          // Remove a syntax specification
          current_syntax.reset();
          syntaxes.erase(key);
        } else {
          current_syntax = std::make_shared<SyntaxParser>(args[2], args[3]);
          syntaxes.emplace(key, current_syntax);
        }
      }
      // e.g. transition from state "normal" to "comment" when the pattern "#"
      // is matched and classify the matched characters as "python_comment"
      //   syntax_pattern normal comment python_comment '#'
      else if (("syntax_pattern" == args[0]) && current_syntax &&
               (5 <= args.size())) {
        Glib::RegexCompileFlags flags{static_cast<Glib::RegexCompileFlags>(0)};
        for (auto cit{args.cbegin() + 5}; cit != args.cend(); ++cit) {
          if ("ignorecase" == *cit) {
            flags |= Glib::RegexCompileFlags::REGEX_CASELESS;
          } else {
            throw std::invalid_argument{"Invalid option for syntax pattern"};
          }
        }
        current_syntax->addPattern(args[1], args[2], args[3],
                                   Glib::Regex::create(args[4], flags));
      }
      // e.g. default to the Python syntax rules when viewing a file ending with
      // ".py" or ".pyw"
      //   syntax_files Python '\.pyw?$'
      else if (("syntax_files" == args[0]) &&
               ((2 == args.size()) || (3 == args.size()))) {
        const auto key{args[1]};
        if (2 == args.size()) {
          // Remove file pattern for a syntax specification
          syntax_file_patterns.erase(key);
        } else {
          Glib::RegexCompileFlags flags{
              static_cast<Glib::RegexCompileFlags>(0)};
          if (isWindows()) {
            flags |= Glib::RegexCompileFlags::REGEX_CASELESS;
          }
          syntax_file_patterns.emplace(key,
                                       Glib::Regex::create(args[2], flags));
        }
      }
    } catch (const std::runtime_error &) {
      std::cerr << "Oh oh, something went wrong here!\n"; // TODO: Improve
    }
  }

  return true;
} // TODO

void Df::Resources::setDifferenceColours(const Glib::ustring &s) {
  const auto preWhitespaceRegexp{Glib::Regex::create("^\\s*")};
  const auto postWhitespaceRegexp{Glib::Regex::create("\\s*$")};

  auto tmp_str{
      preWhitespaceRegexp->replace(s, 0, "", Glib::REGEX_MATCH_NOTEMPTY)};
  tmp_str =
      postWhitespaceRegexp->replace(tmp_str, 0, "", Glib::REGEX_MATCH_NOTEMPTY);

  difference_colours = Glib::Regex::split_simple("\\s+", tmp_str);
}

void Df::Resources::setKeyBinding(const Glib::ustring &ctx,
                                  const Glib::ustring &s,
                                  const Glib::ustring &v) {}

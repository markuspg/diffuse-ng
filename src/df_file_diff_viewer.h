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

#ifndef DF_FILE_DIFF_VIEWER_H
#define DF_FILE_DIFF_VIEWER_H

#include "df_preferences.h"

#include <gtkmm/table.h>

#include <variant>

namespace Diffuse {
using Options =
    std::map<Glib::ustring, std::variant<unsigned long, Glib::ustring>>;

/**
 * @brief Widget used to compare and merge text files
 */
class FileDiffViewer : public Gtk::Table {
public:
  /**
   * @brief The FileDiffViewer is always in one of these modes defining its
   *   cursor and hotkey behaviour
   */
  enum class Mode {
    LINE,
    CHAR,
    ALIGN,
  };

  /**
   * @brief Class describing a single line of a pane
   */
  class Line {
  public:
    Line(std::optional<unsigned> &line_number,
         std::optional<Glib::ustring> &text);

    /**
     * @brief Returns the current text for this line
     * @return The current text of this line
     */
    std::optional<Glib::ustring> getText() const;

  private:
    //! Cache used to speed up the comparison of strings (this should be cleared
    //! whenever the comparison preferences change)
    std::optional<void *> compare_string;
    //! Flag indicating if modifications are present
    bool is_modified = false;
    //! Line number
    std::optional<unsigned> line_number;
    //! Actual modified text
    std::optional<Glib::ustring> modified_text;
    //! Original text of the line
    std::optional<Glib::ustring> text;
  };

  /**
   * @brief Class describing a text pane
   */
  class Pane {
    //! Cache of character differences for each line; diff_cache[i] corresponds
    //! to lines[i]; portion of the cache are cleared by settings entries to
    //! nullptr
    std::vector<void *> diff_cache;
    //! Mask indicating the type of line endings present
    unsigned int format{0};
    //! High water mark for line length in pango units (used to determine the
    //! required horizontal scroll range)
    unsigned line_lengths{0};
    //! List of lines displayed in this pane (including spacing lines)
    std::vector<void *> lines;
    //! Highest line number
    unsigned int max_line_number{0};
    //! Number of lines with edits
    unsigned int num_edits{0};
    //! Cache of syntax highlighting information for each line; syntax_cache[i]
    //! corresponds to lines[i]; the list is truncated when a change to a line
    //! invalidates a portion of the cache
    std::vector<void *> syntax_cache;
  };

  FileDiffViewer(guint n, Preferences &prefs);

  bool hasDifferences() const;
  void setOptions(const Options &options);

private:
  void align_mode_enter_line_mode();
  void align_text();
  void extend_first_line();
  void extend_last_line();
  void first_line();
  void last_line();
  void line_mode_down(void *selection = nullptr);
  void line_mode_enter_align_mode();
  void line_mode_extend_up();
  void line_mode_left(void *selection = nullptr);
  void line_mode_page_down(void *selection = nullptr);
  void line_mode_page_up(void *selection = nullptr);
  void line_mode_right(void *selection = nullptr);
  void line_mode_up(void *selection = nullptr);
  void setCharMode();
  void setLineMode();

  //! Diff blocks
  std::vector<void *> blocks;

  // Cached data
  std::optional<void *> map_cache;
  std::optional<void *> syntax;

  // Keybindings
  std::map<Glib::ustring, void (FileDiffViewer::*)()> align_mode_actions;
  std::map<Glib::ustring, void (FileDiffViewer::*)()> character_mode_actions;
  std::map<Glib::ustring, void (FileDiffViewer::*)()> line_mode_actions;

  //! Editing mode
  Mode mode{Mode::LINE};

  int align_line = 0;
  int align_pane = 0;
  int current_char = 0;
  int current_line = 0;
  int current_pane = 1;
  int cursor_column = -1;
  Options options;
  Preferences &prefs;
  int selection_char = 0;
  int selection_line = 0;
  std::map<void *, void *> string_width_cache;

  // Undo-related data
  std::vector<void *> redos;
  std::optional<void *> undoblock;
  std::vector<void *> undos;
};
} // namespace Diffuse

#endif // DF_FILE_DIFF_VIEWER_H

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

namespace Diffuse {
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

  FileDiffViewer(guint n, Preferences &prefs);

  bool hasDifferences() const;

private:
  void extend_first_line();
  void extend_last_line();
  void first_line();
  void last_line();
  void line_mode_enter_align_mode();
  void line_mode_extend_up();
  void line_mode_up(void *selection = nullptr);
  void setCharMode();

  //! Diff blocks
  std::vector<void *> blocks;

  // Cached data
  std::optional<void *> map_cache;
  std::optional<void *> syntax;

  // Keybindings
  std::map<Glib::ustring, void (FileDiffViewer::*)()> line_mode_actions;

  //! Editing mode
  Mode mode{Mode::LINE};

  int align_line = 0;
  int align_pane = 0;
  int current_char = 0;
  int current_line = 0;
  int current_pane = 1;
  int cursor_column = -1;
  std::map<void *, void *> options;
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

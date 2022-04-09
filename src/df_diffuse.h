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

#ifndef DF_DIFFUSE_H
#define DF_DIFFUSE_H

#include "df_file_diff_viewer.h"

#include <gtkmm/notebook.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/window.h>

#include <variant>

namespace Diffuse {
/**
 * @brief Main application class containing a set of file viewers
 *
 * This class displays tabs for switching between viewers and dispatches menu
 * commands to the current viewer.
 */
class Diffuse : public Gtk::Window {
public:
  /**
   * @brief Specialisation of FileDiffViewer for Diffuse
   */
  class FileDiffViewer : public ::Diffuse::FileDiffViewer {
  public:
    /**
     * @brief Pane footer
     */
    class PaneFooter : public Gtk::HBox {
    public:
      PaneFooter();

      void setEncoding(const std::optional<Glib::ustring> &s);

    private:
      Gtk::Label cursor;
      Gtk::Label encoding;
      Gtk::Label format;
    };

    using type_signal_status_changed = sigc::signal<void()>;
    using type_signal_syntax_changed = sigc::signal<void()>;
    using type_signal_title_changed = sigc::signal<void()>;
    type_signal_status_changed signal_status_changed();
    type_signal_syntax_changed signal_syntax_changed();
    type_signal_title_changed signal_title_changed();

    FileDiffViewer(guint n, Preferences &prefs, const Glib::ustring &title);

  protected:
    type_signal_status_changed m_status_changed;
    type_signal_syntax_changed m_syntax_changed;
    type_signal_title_changed m_title_changed;
  };

  using Encoding = std::optional<Glib::ustring>;
  using Labels = std::vector<Glib::ustring>;
  struct Revision {
    std::optional<Glib::ustring> revision;
    Encoding encoding;
  };
  using Revisions = std::vector<Revision>;
  struct Specification {
    std::optional<std::string> path;
    Revisions revs;
  };
  using Specs = std::vector<Specification>;
  using LabelledSpecs =
      std::vector<std::pair<Specification, std::optional<Glib::ustring>>>;

  Diffuse(const std::string &rc_dir);

  void closeOnSame();
  void createCommitFileTabs(const Specs &items, const Labels &labels,
                            const Options &options);
  void createModifiedFileTabs(const Specs &items, const Labels &labels,
                              const Options &options);
  void createSeparateTabs(const Specs &items, const Labels &labels,
                          const Options &options);
  void createSingleTab(const Specs &items, const Labels &labels,
                       const Options &options);
  bool loadState(const std::string &statepath);
  void newLoadedFileDiffViewer(const LabelledSpecs &items);
  void preferences_updated();
  bool saveState(const std::string &statepath);

  Gtk::Notebook notebook;
  Preferences prefs;

private:
  bool configure_cb(const GdkEventConfigure *event);
  bool confirmQuit();
  bool delete_cb(const GdkEventAny *event);
  bool focus_in_cb(GdkEventFocus *event);
  std::unique_ptr<FileDiffViewer> newFileDiffViewer(guint n);
  bool notebooktab_button_press_cb(GdkEventButton *event,
                                   FileDiffViewer *viewer);
  void remove_tab_cb(FileDiffViewer *viewer);
  void status_changed_cb();
  void syntax_changed_cb();
  void switch_page_cb(Gtk::Widget *ptr, guint page_num);
  void title_changed_cb();
  bool window_state_cb(const GdkEventWindowState *event);

  // State information that should persist across sessions
  std::map<Glib::ustring, bool> bool_state;
  std::map<Glib::ustring, int> int_state;
  // Search history is application-wide
  std::vector<void *> search_history;
  std::optional<void *> search_pattern;
  //! Number of created viewers (used to label some tabs)
  unsigned viewer_count = 0u;

  std::vector<void *> closed_tabs;
  Gtk::Statusbar statusbar;
};
} // namespace Diffuse

#endif // DF_DIFFUSE_H

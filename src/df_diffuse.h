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

#ifndef DF_DIFFUSE_H
#define DF_DIFFUSE_H

#include "df_file_diff_viewer.h"
#include "df_file_info.h"
#include "df_preferences.h"

#include <gtkmm/box.h>
#include <gtkmm/notebook.h>
#include <gtkmm/window.h>

#include <optional>
#include <variant>
#include <vector>

namespace Diffuse {
using Encoding = std::optional<Glib::ustring>;
using Labels = std::vector<Glib::ustring>;
using Options =
    std::map<Glib::ustring, std::variant<Glib::ustring, unsigned long>>;
struct Revision {
  Revision(const std::optional<Glib::ustring> &rev,
           std::optional<Glib::ustring> &enc)
      : revision{rev}, encoding{enc} {}

  std::optional<Glib::ustring> revision;
  Encoding encoding;
};
using Revisions = std::vector<Revision>;
struct Specification {
  Specification(const std::string &fn, const Revisions &revs)
      : filename{fn}, revisions{revs} {}
  Specification(const std::optional<std::string> &fn, const Revisions &revs)
      : filename{fn}, revisions{revs} {}

  std::optional<std::string> filename;
  Revisions revisions;
};

using Specs = std::vector<Specification>;

class Diffuse : public Gtk::Window {
public:
  /**
   * @brief Specialization of FileDiffViewer for Diffuse
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

    /**
     * @brief Pane header
     */
    class PaneHeader : public Gtk::HBox {
    public:
      PaneHeader();

      void setEdits(bool has_edits);

    private:
      void button_cb();
      void updateTitle();

      bool has_edits = false;
      //! File's name and information about how to retrieve it from a VCS
      FileInfo info;
      Gtk::Label label;
    };

    FileDiffViewer(std::size_t n, Preferences &prefs,
                   const Glib::ustring &title);

  private:
    std::vector<void *> footers;
    std::vector<void *> headers;
    Glib::ustring status;
    const Glib::ustring title;
  };

  Diffuse(const Glib::ustring &rc_dir);

  void createCommitFileTabs(const Specs &items, const Labels &labels,
                            const Options &options);
  void createModifiedFileTabs(const Specs &items, const Labels &labels,
                              const Options &options);
  void createSeparateTabs(const Specs &items, const Labels &labels,
                          const Options &options);
  void createSingleTab(const Specs &items, const Labels &labels,
                       const Options &options);

  void closeOnSame();
  bool loadState(const std::string &statepath);
  void newLoadedFileDiffViewer(const std::vector<void *> &items);
  void preferences_updated();
  void saveState(const std::string &statepath);

  Gtk::Notebook notebook;
  Preferences prefs;

private:
  bool configure_cb(const GdkEventConfigure *ev);
  bool confirmQuit();
  bool delete_cb(const GdkEventAny *ev);
  bool window_state_cb(const GdkEventWindowState *ev);

  // State information that should persist across sessions
  std::map<Glib::ustring, bool> bool_state{{"search_backwards", false},
                                           {"search_matchcase", false},
                                           {"window_maximized", false}};
  std::map<Glib::ustring, int> int_state{{"window_height", 768},
                                         {"window_width", 1024}};

  // Search history is application-wide
  std::vector<void *> search_history;
  void *search_pattern = nullptr;

  //! VBox for the contents
  Gtk::VBox vbox;

  //! Number of created viewers (used to label some tabs)
  unsigned int viewer_count = 0;
};

using TabCreationFunc = void (Diffuse::*)(const Specs &, const Labels &,
                                          const Options &);
using TabCreationFuncMap = std::map<Glib::ustring, TabCreationFunc>;
} // namespace Diffuse

#endif // DF_DIFFUSE_H

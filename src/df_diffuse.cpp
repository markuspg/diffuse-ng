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

#include "df_diffuse.h"

// #include <gtkmm/accelgroup.h>
#include <gtkmm/iconfactory.h>
#include <gtkmm/main.h>
#include <gtkmm/separator.h>
#include <gtkmm/stock.h>

#include <gdkmm/general.h>

#include <glibmm/convert.h>
#include <glibmm/miscutils.h>

namespace Df = Diffuse;

static void appendButtons(Gtk::HBox &box, Gtk::BuiltinIconSize size,
                          const std::vector<void *> &specs);

Df::Diffuse::Diffuse(const Glib::ustring &rc_dir)
    : prefs{Glib::build_filename(rc_dir, Glib::locale_from_utf8("prefs"))} {
  int_state["window_x"] =
      std::max(0, Gdk::screen_width() - (int_state["window_width"] / 2));
  int_state["window_y"] =
      std::max(0, Gdk::screen_height() - (int_state["window_height"] / 2));

  signal_configure_event().connect(
      sigc::mem_fun1(this, &Diffuse::configure_cb));
  signal_window_state_event().connect(
      sigc::mem_fun1(this, &Diffuse::window_state_cb));
  signal_delete_event().connect(sigc::mem_fun1(this, &Diffuse::delete_cb));

  // Gtk::AccelGroup accel_group;

  // Create some custom icons for merging
  const char *const DIFFUSE_STOCK_NEW_2WAY_MERGE = "diffuse-new-2way-merge";
  const char *const DIFFUSE_STOCK_NEW_3WAY_MERGE = "diffuse-new-3way-merge";
  const char *const DIFFUSE_STOCK_LEFT_RIGHT = "diffuse-left-right";
  const char *const DIFFUSE_STOCK_RIGHT_LEFT = "diffuse-right-left";

  auto factory = Gtk::IconFactory::create();
  // Render the base item used to indicate a new document
  auto p0 = render_icon(Gtk::Stock::NEW, Gtk::ICON_SIZE_LARGE_TOOLBAR);
  auto h = p0->get_height();
  auto w = p0->get_width();

  // Render new 2-way merge icon
  auto s = 0.8;
  auto sh = static_cast<int>(s * h);
  auto sw = static_cast<int>(s * w);
  auto h1 = h - sh;
  auto w1 = w - sw;
  auto p = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h);
  p->fill(0);
  p0->composite(p, 0, 0, sw, sh, 0, 0, s, s, Gdk::INTERP_BILINEAR, 255);
  p0->composite(p, w1, h1, sw, sh, w1, h1, s, s, Gdk::INTERP_BILINEAR, 255);
  factory->add(Gtk::StockID{DIFFUSE_STOCK_NEW_2WAY_MERGE}, Gtk::IconSet(p));

  // Render new 3-way merge icon
  s = 0.7;
  sh = static_cast<int>(s * h);
  sw = static_cast<int>(s * w);
  h1 = (h - sh) / 2;
  w1 = (w - sw) / 2;
  auto h2 = h - sh;
  auto w2 = w - sw;
  p = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h);
  p->fill(0);
  p0->composite(p, 0, 0, sw, sh, 0, 0, s, s, Gdk::INTERP_BILINEAR, 255);
  p0->composite(p, w1, h1, sw, sh, w1, h1, s, s, Gdk::INTERP_BILINEAR, 255);
  p0->composite(p, w2, h2, sw, sh, w2, h2, s, s, Gdk::INTERP_BILINEAR, 255);
  factory->add(Gtk::StockID{DIFFUSE_STOCK_NEW_3WAY_MERGE}, Gtk::IconSet{p});

  // Render the left and right arrow which will be used in the custom icons
  p0 = render_icon(Gtk::Stock::GO_FORWARD, Gtk::ICON_SIZE_LARGE_TOOLBAR);
  auto p1 = render_icon(Gtk::Stock::GO_BACK, Gtk::ICON_SIZE_LARGE_TOOLBAR);
  h = p0->get_height();
  w = p0->get_width();
  s = 0.65;
  sh = static_cast<int>(s * h);
  sw = static_cast<int>(s * w);
  h1 = h - sh;
  w1 = w - sw;

  // Create merge from left then right icon
  p = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h);
  p->fill(0);
  p1->composite(p, w1, h1, sw, sh, w1, h1, s, s, Gdk::INTERP_BILINEAR, 255);
  p0->composite(p, 0, 0, sw, sh, 0, 0, s, s, Gdk::INTERP_BILINEAR, 255);
  factory->add(Gtk::StockID{DIFFUSE_STOCK_LEFT_RIGHT}, Gtk::IconSet{p});

  // Create merge from right then left icon
  p = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h);
  p->fill(0);
  p0->composite(p, 0, h1, sw, sh, 0, h1, s, s, Gdk::INTERP_BILINEAR, 255);
  p1->composite(p, w1, 0, sw, sh, w1, 0, s, s, Gdk::INTERP_BILINEAR, 255);
  factory->add(Gtk::StockID{DIFFUSE_STOCK_RIGHT_LEFT}, Gtk::IconSet{p});

  // Make the icons available for use
  factory->add_default();

  struct MenuSpecBase {};
  struct MenuSpecEntry {
    const Glib::ustring label;
    sigc::mem_functor0<void, Diffuse> callback;
    std::optional<Glib::ustring> smthng;
    std::optional<Gtk::StockID> icon;
    const Glib::ustring action;
  };
  struct MenuSpecSeparator : public MenuSpecBase {};
  struct MenuSpec {
    const Glib::ustring category;
    const std::vector<std::variant<MenuSpecEntry, MenuSpecSeparator>> specs;
  };
  using MenuSpecs = std::vector<MenuSpec>;
  MenuSpecs menuspecs{
      {MenuSpec{
          "_File",
          {MenuSpecEntry{
               "_Open File...",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::open_file_cb),
               std::nullopt, Gtk::Stock::OPEN, "open_file"},
           MenuSpecEntry{"Open File In New _Tab...",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::open_file_in_new_tab_cb),
                         std::nullopt, std::nullopt, "open_file_in_new_tab"},
           MenuSpecEntry{"Open _Modified Files...",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::open_modified_files_cb),
                         std::nullopt, std::nullopt, "open_modified_files"},
           MenuSpecEntry{"Open Commi_t...",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::open_commit_cb),
                         std::nullopt, std::nullopt, "open_commit"},
           MenuSpecEntry{"_Reload File",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::reload_file_cb),
                         std::nullopt, Gtk::Stock::REFRESH, "reload_file"},
           MenuSpecSeparator{},
           MenuSpecEntry{
               "_Save File",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::save_file_cb),
               std::nullopt, Gtk::Stock::SAVE, "save_file"},
           MenuSpecEntry{"Save File _As...",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::save_file_as_cb),
                         std::nullopt, Gtk::Stock::SAVE_AS, "save_file_as"},
           MenuSpecEntry{
               "Save A_ll",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::save_all_cb),
               std::nullopt, std::nullopt, "save_all"},
           MenuSpecSeparator{},
           MenuSpecEntry{"New _2-Way File Merge",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::new_2_way_file_merge_cb),
                         std::nullopt,
                         Gtk::StockID{DIFFUSE_STOCK_NEW_2WAY_MERGE},
                         "new_2_way_file_merge"},
           MenuSpecEntry{"New _3-Way File Merge",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::new_3_way_file_merge_cb),
                         std::nullopt,
                         Gtk::StockID{DIFFUSE_STOCK_NEW_3WAY_MERGE},
                         "new_3_way_file_merge"},
           MenuSpecEntry{"New _N-Way File Merge...",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::new_n_way_file_merge_cb),
                         std::nullopt, std::nullopt, "new_n_way_file_merge"},
           MenuSpecSeparator{},
           MenuSpecEntry{
               "_Close Tab",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::close_tab_cb),
               std::nullopt, Gtk::Stock::CLOSE, "close_tab"},
           MenuSpecEntry{"_Undo Close Tab",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::undo_close_tab_cb),
                         std::nullopt, std::nullopt, "undo_close_tab"},
           MenuSpecEntry{
               "_Quit",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::quit_cb),
               nullptr, Gtk::Stock::QUIT, "quit"}}}},
      MenuSpec{
          "_Edit",
          {MenuSpecEntry{
               "_Undo",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "undo", Gtk::Stock::UNDO, "undo"},
           MenuSpecEntry{
               "_Redo",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "redo", Gtk::Stock::REDO, "redo"},
           MenuSpecSeparator{},
           MenuSpecEntry{
               "Cu_t",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "cut", Gtk::Stock::CUT, "cut"},
           MenuSpecEntry{
               "_Copy",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "copy", Gtk::Stock::COPY, "copy"},
           MenuSpecEntry{
               "_Paste",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "paste", Gtk::Stock::PASTE, "paste"},
           MenuSpecSeparator{},
           MenuSpecEntry{
               "Select _All",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "select_all", std::nullopt, "select_all"},
           MenuSpecEntry{
               "C_lear Edits",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "clear_edits", Gtk::Stock::CLEAR, "clear_edits"},
           MenuSpecEntry{
               "_Dismiss All Edits",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::button_cb),
               "dismiss_all_edits", std::nullopt, "dismiss_all_edits"},
           MenuSpecSeparator{},
           MenuSpecEntry{
               "_Find...",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::find_cb),
               std::nullopt, Gtk::Stock::FIND, "find"},
           MenuSpecEntry{
               "Find _Next",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::find_next_cb),
               std::nullopt, std::nullopt, "find_next"},
           MenuSpecEntry{"Find Pre_vious",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::find_previous_cb),
                         std::nullopt, std::nullopt, "find_previous"},
           MenuSpecEntry{
               "_Go To Line...",
               sigc::mem_fun0<void, Df::Diffuse>(this, &Diffuse::go_to_line_cb),
               std::nullopt, Gtk::Stock::JUMP_TO, "go_to_line"},
           MenuSpecSeparator{},
           MenuSpecEntry{"Pr_eferences...",
                         sigc::mem_fun0<void, Df::Diffuse>(
                             this, &Diffuse::preferences_cb),
                         std::nullopt, Gtk::Stock::PREFERENCES,
                         "preferences"}}}};
}

/**
 * @brief Callback for most menu items and buttons
 */
void Df::Diffuse::button_cb() {}

/**
 * @brief Callback for the close tab menu item
 */
void Df::Diffuse::close_tab_cb() {}

/**
 * @brief Close all tabs without differences
 */
void Df::Diffuse::closeOnSame() {}

/**
 * @brief Record the window's position and size
 * @param[in] ev The event that triggered the slot carrying all relevant data
 * @return true always
 */
bool Df::Diffuse::configure_cb(const GdkEventConfigure *const ev) {
  // Read the state directly instead of using window_maximized as the order of
  // configure/window state events is undefined
  if (0 ==
      (get_window()->get_state() & Gdk::WindowState::WINDOW_STATE_MAXIMIZED)) {
    int x;
    int y;
    get_window()->get_root_origin(x, y);
    int_state["window_x"] = x;
    int_state["window_y"] = y;
    int_state["window_width"] = ev->width;
    int_state["window_height"] = ev->height;
  }

  return true;
}

/**
 * @brief Returns True if the application can safely quit
 * @return
 */
bool Df::Diffuse::confirmQuit() { return true; }

void Df::Diffuse::createCommitFileTabs(const Specs &items, const Labels &labels,
                                       const Options &options) {}

void Df::Diffuse::createModifiedFileTabs(const Specs &items,
                                         const Labels &labels,
                                         const Options &options) {}

void Df::Diffuse::createSeparateTabs(const Specs &items, const Labels &labels,
                                     const Options &options) {}

void Df::Diffuse::createSingleTab(const Specs &items, const Labels &labels,
                                  const Options &options) {}

/**
 * @brief Respond to close window request from the window manager
 * @param[in] ev The event that triggered the slot carrying all relevant data
 * @return false is the application can close or true otherwise
 */
bool Df::Diffuse::delete_cb(const GdkEventAny *const ev) {
  if (confirmQuit()) {
    Gtk::Main::quit();
    return false;
  }

  return true;
}

/**
 * @brief Callback for the find menu item
 */
void Df::Diffuse::find_cb() {}

/**
 * @brief Callback for the find next menu item
 */
void Df::Diffuse::find_next_cb() {}

/**
 * @brief Callback for the find previous menu item
 */
void Df::Diffuse::find_previous_cb() {}

/**
 * @brief Callback for the go to line menu item
 */
void Df::Diffuse::go_to_line_cb() {}

bool Df::Diffuse::loadState(const std::string &statepath) { return true; }

/**
 * @brief Callback for the new 2-way file merge menu item
 */
void Df::Diffuse::new_2_way_file_merge_cb() {}

/**
 * @brief Callback for the new 3-way file merge menu item
 */
void Df::Diffuse::new_3_way_file_merge_cb() {}

/**
 * @brief Callback for the new n-way file merge menu item
 */
void Df::Diffuse::new_n_way_file_merge_cb() {}

void Df::Diffuse::newLoadedFileDiffViewer(const std::vector<void *> &items) {}

/**
 * @brief Callback for the open commit menu item
 */
void Df::Diffuse::open_commit_cb() {}

/**
 * @brief Callback for the open file menu item
 */
void Df::Diffuse::open_file_cb() {}

/**
 * @brief Callback for the open file menu item
 */
void Df::Diffuse::open_file_in_new_tab_cb() {}

/**
 * @brief Callback for the open modified files menu item
 */
void Df::Diffuse::open_modified_files_cb() {}

/**
 * @brief Callback for the preferences menu item
 */
void Df::Diffuse::preferences_cb() {}

/**
 * @brief Notify all viewers to changes to the preferences
 */
void Df::Diffuse::preferences_updated() {}

/**
 * @brief Callback for the quit menu item
 */
void Df::Diffuse::quit_cb() {}

/**
 * @brief Callback for the reload file menu item
 */
void Df::Diffuse::reload_file_cb() {}

/**
 * @brief Callback for the save all menu item
 */
void Df::Diffuse::save_all_cb() {}

/**
 * @brief Callback for the save file as menu item
 */
void Df::Diffuse::save_file_as_cb() {}

/**
 * @brief Callback for the save file menu item
 */
void Df::Diffuse::save_file_cb() {}

void Df::Diffuse::saveState(const std::string &statepath) {}

/**
 * @brief Callback for the undo close tab menu item
 */
void Df::Diffuse::undo_close_tab_cb() {}

/**
 * @brief Record the window's maximised state
 * @param[in] ev The event that triggered the slot carrying all relevant data
 * @return true always
 */
bool Df::Diffuse::window_state_cb(const GdkEventWindowState *const ev) {
  bool_state["window_maximized"] =
      0 != (ev->new_window_state & Gdk::WINDOW_STATE_MAXIMIZED);

  return true;
}

Df::Diffuse::FileDiffViewer::FileDiffViewer(std::size_t n,
                                            Df::Preferences &prefs,
                                            const Glib::ustring &title)
    : ::Diffuse::FileDiffViewer{n, prefs}, title{title} {}

Df::Diffuse::FileDiffViewer::PaneFooter::PaneFooter() {
  pack_start(cursor, false, false, 0);
  cursor.show();

  Gtk::VSeparator separator;
  pack_end(separator, false, false, 10);
  separator.show();

  pack_end(encoding, false, false, 0);
  encoding.show();

  Gtk::VSeparator separator_2;
  pack_end(separator_2, false, false, 10);
  separator_2.show();

  pack_end(format, false, false, 0);
  format.show();

  Gtk::VSeparator separator_3;
  pack_end(separator_3, false, false, 10);
  separator_3.show();

  int h, w;
  get_size_request(w, h);
  set_size_request(0, h);
}

/**
 * @brief set the format label
 * @param s
 */
void Df::Diffuse::FileDiffViewer::PaneFooter::setEncoding(
    const std::optional<Glib::ustring> &s) {
  encoding.set_text(s.has_value() ? s.value() : "");
}

Df::Diffuse::FileDiffViewer::PaneHeader::PaneHeader() {
  appendButtons(
      *this, Gtk::ICON_SIZE_MENU,
      {
          /*[ gtk.STOCK_OPEN, self.button_cb, 'open', _('Open File...') ],
                         [ gtk.STOCK_REFRESH, self.button_cb, 'reload',
             _('Reload File') ], [ gtk.STOCK_SAVE, self.button_cb, 'save',
             _('Save File') ], [ gtk.STOCK_SAVE_AS, self.button_cb, 'save_as',
             _('Save File As...') ] */
      });

  int h, w;
  label.get_size_request(w, h);
  label.set_size_request(0, h);
  label.set_selectable(true);
  pack_start(label, true, true, 0);
  label.show();

  updateTitle();
}

/**
 * @brief Callback for buttons
 */
void Df::Diffuse::FileDiffViewer::PaneHeader::button_cb() {
  // emit(s)
}

/**
 * @brief Set num edits
 * @param has_edts
 */
void Df::Diffuse::FileDiffViewer::PaneHeader::setEdits(const bool has_edts) {
  if (has_edits != has_edts) {
    has_edits = has_edts;
    updateTitle();
  }
}

/**
 * @brief Create an appropriate title for the pane header
 */
void Df::Diffuse::FileDiffViewer::PaneHeader::updateTitle() {
  std::vector<Glib::ustring> ss;
  if (info.getLabel()) {
    // Show the provided label instead of the file name
    ss.emplace_back(info.getLabel().value());
  } else {
    if (info.getName()) {
      ss.emplace_back(info.getName().value());
    }
    if (info.getRevision()) {
      ss.emplace_back("(" + info.getRevision().value() + ")");
    }
    if (has_edits) {
      ss.emplace_back("*");
    }
    Glib::ustring s;
    Glib::ustring::size_type i = 0;
    while (i < ss.size()) {
      s += ss[i];
      ++i;
      if (i < ss.size()) {
        s += ' ';
      }
    }
    label.set_text(s);
    // set_tooltip(self.label, s)
    // self.emit('title_changed')
  }
}

/**
 * @brief Convenience method for packing buttons into a container according to a
 *   template
 * @param box
 * @param size
 * @param specs
 */
static void appendButtons(Gtk::HBox &box, Gtk::BuiltinIconSize size,
                          const std::vector<void *> &specs) {}

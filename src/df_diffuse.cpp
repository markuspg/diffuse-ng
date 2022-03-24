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

#include "df_diffuse.h"
#include "df_about_dialog.h"
#include "df_globals.h"
#include "df_notebook_tab.h"
#include "df_vcss.h"

#include <gtkmm/accelgroup.h>
#include <gtkmm/iconfactory.h>
#include <gtkmm/stock.h>

#include <glibmm/miscutils.h>

#include <fstream>
#include <iostream>

namespace Df = Diffuse;

static Df::Diffuse::LabelledSpecs
assign_file_labels(const Df::Diffuse::Specs &items,
                   const Df::Diffuse::Labels &labels);

static Df::VCSs theVCSs;

Df::Diffuse::Diffuse(const std::string &rc_dir)
    : prefs{Glib::build_filename(rc_dir, Glib::locale_from_utf8("prefs"))},
      bool_state{{"search_backwards", false},
                 {"search_matchcase", false},
                 {"window_maximized", false}},
      int_state{{"window_height", 768}, {"window_width", 1024}} {
  int_state["window_x"] = std::max(
      0, (Gdk::Screen::get_default()->get_width() - int_state["window_width"]) /
             2);
  int_state["window_y"] =
      std::max(0, (Gdk::Screen::get_default()->get_height() -
                   int_state["window_height"]) /
                      2);

  signal_configure_event().connect(
      sigc::mem_fun(*this, &Diffuse::configure_cb));
  signal_window_state_event().connect(
      sigc::mem_fun(*this, &Diffuse::window_state_cb));
  signal_delete_event().connect(sigc::mem_fun(*this, &Diffuse::delete_cb));

  const auto accel_group{Gtk::AccelGroup::create()};

  // Create a VBox for the contents
  Gtk::VBox vbox;

  // Create some custom icons for merging
  constexpr auto DIFFUSE_STOCK_NEW_2WAY_MERGE{"diffuse-new-2way-merge"};
  constexpr auto DIFFUSE_STOCK_NEW_3WAY_MERGE{"diffuse-new-3way-merge"};
  constexpr auto DIFFUSE_STOCK_LEFT_RIGHT{"diffuse-left-right"};
  constexpr auto DIFFUSE_STOCK_RIGHT_LEFT{"diffuse-right-left"};

  const auto factory{Gtk::IconFactory::create()};
  // Render the base item used to indicate a new document
  auto p0{render_icon(Gtk::Stock::NEW, Gtk::ICON_SIZE_LARGE_TOOLBAR)};
  auto h = p0->get_height();
  auto w = p0->get_width();

  // Render new 2-way merge icon
  auto s{0.8};
  auto sh{static_cast<int>(s) * h};
  auto sw{static_cast<int>(s) * w};
  auto h1{h - sh};
  auto w1{w - sw};
  auto p{Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h)};
  p->fill(0);
  p0->composite(p, 0, 0, sw, sh, 0, 0, s, s, Gdk::INTERP_BILINEAR, 255);
  p0->composite(p, w1, h1, sw, sh, w1, h1, s, s, Gdk::INTERP_BILINEAR, 255);
  factory->add(Gtk::StockID{DIFFUSE_STOCK_NEW_2WAY_MERGE}, Gtk::IconSet{p});

  // Render new 3-way merge icon
  s = 0.7;
  sh = static_cast<int>(s) * h;
  sw = static_cast<int>(s) * w;
  h1 = (h - sh) / 2;
  w1 = (w - sw) / 2;
  const auto h2{h - sh};
  const auto w2{w - sw};
  p = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, w, h);
  p->fill(0);
  p0->composite(p, 0, 0, sw, sh, 0, 0, s, s, Gdk::INTERP_BILINEAR, 255);
  p0->composite(p, w1, h1, sw, sh, w1, h1, s, s, Gdk::INTERP_BILINEAR, 255);
  p0->composite(p, w2, h2, sw, sh, w2, h2, s, s, Gdk::INTERP_BILINEAR, 255);
  factory->add(Gtk::StockID{DIFFUSE_STOCK_NEW_3WAY_MERGE}, Gtk::IconSet{p});

  // Render the left and right arrow which will be used in the custom icons
  p0 = render_icon(Gtk::Stock::GO_FORWARD, Gtk::ICON_SIZE_LARGE_TOOLBAR);
  const auto p1{render_icon(Gtk::Stock::GO_BACK, Gtk::ICON_SIZE_LARGE_TOOLBAR)};
  h = p0->get_height();
  s = 0.65;
  w = p0->get_width();
  sh = static_cast<int>(s) * h;
  sw = static_cast<int>(s) * w;
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

  notebook.set_scrollable(true);
  // notebook.signal_switch_page().connect(sigc::mem_fun(*this,
  // &Diffuse::switch_page_cb));
  vbox.pack_start(notebook, true, true, 0);
  notebook.show();

  // Add a statusbar to the bottom
  vbox.pack_start(statusbar, false, false, 0);
  statusbar.show();

  add_accel_group(accel_group);
  add(vbox);
  vbox.show();
  signal_focus_in_event().connect(sigc::mem_fun(*this, &Diffuse::focus_in_cb));
}

/**
 * @brief Close all tabs without differences
 */
void Df::Diffuse::closeOnSame() {
  const auto n{notebook.get_n_pages()};
  for (auto i{0}; i < n; ++i) {
    const auto fdv{
        dynamic_cast<::Df::FileDiffViewer *>(notebook.get_nth_page(i))};
    if ((fdv) && !fdv->hasDifferences()) {
      notebook.remove_page(i);
    }
  }
}

/**
 * @brief Record the window's position and size
 * @param[in] event The GdkEventConfigure triggered by a window resizing
 * @return _false_ in any case
 */
bool Df::Diffuse::configure_cb(const GdkEventConfigure *const event) {
  // Read the state directly instead of using window_maximized as the order of
  // configure/window state events is undefined
  const auto window = get_window();
  if (0 == (window->get_state() & Gdk::WindowState::WINDOW_STATE_MAXIMIZED)) {
    int x, y;
    window->get_root_origin(x, y);
    int_state["window_x"] = x;
    int_state["window_y"] = y;
    int_state["window_height"] = event->height;
    int_state["window_width"] = event->width;
  }

  return false;
}

/**
 * @brief Check if the application can quit safely
 * @return _true_ if the application can quit safely, _false_ otherwise
 */
bool Df::Diffuse::confirmQuit() { return true; }

void Df::Diffuse::createCommitFileTabs(const Specs &items, const Labels &labels,
                                       const Options &options) {}

void Df::Diffuse::createModifiedFileTabs(const Specs &items,
                                         const Labels &labels,
                                         const Options &options) {}

void Df::Diffuse::createSeparateTabs(const Specs &items, const Labels &labels,
                                     const Options &options) {}

/**
 * @brief Create a new viewer for 'items'
 * @param items
 * @param labels
 * @param options
 */
void Df::Diffuse::createSingleTab(const Specs &items, const Labels &labels,
                                  const Options &options) {
  if (!items.empty()) {
  }
}

/**
 * @brief Respond to close window request from the window manager
 * @param[in] event The GdkEventAny triggered by the close of a top-level window
 * @return _false_ if the application shall quit, _true_ otherwise
 */
bool Df::Diffuse::delete_cb([[maybe_unused]] const GdkEventAny *const event) {
  if (confirmQuit()) {
    gtk_main_quit();
    return false;
  }
  return true;
}

/**
 * @brief Notifies all viewers on focus changes so they may check for external
 *   changes to files
 * @param[in] event The GdkEventFocus signalling that the keyboard focus entered
 *   the window
 * @return _false_ always
 */
bool Df::Diffuse::focus_in_cb(GdkEventFocus *event) {
  return false; // TODO
}

/**
 * @brief Load state information that should persist across sessions
 * @param[in] statepath Path to the state information file
 * @return _true_ on success, _false_ otherwise
 */
bool Df::Diffuse::loadState(const std::string &statepath) {
  return true;
} // TODO

/**
 * @brief Create an empty viewer with "n" panes
 * @param[in] n The number of panes the viewer shall have
 */
std::unique_ptr<Df::Diffuse::FileDiffViewer>
Df::Diffuse::newFileDiffViewer(const guint n) {
  ++viewer_count;
  const Glib::ustring tabname{_("File Merge %1"), viewer_count};
  NotebookTab tab{tabname, Gtk::Stock::FILE};
  auto viewer{std::make_unique<Df::Diffuse::FileDiffViewer>(n, prefs, tabname)};
  tab.button.signal_clicked().connect(
      sigc::bind(sigc::mem_fun(*this, &Diffuse::remove_tab_cb), viewer.get()));
  tab.signal_button_press_event().connect(
      sigc::bind(sigc::mem_fun(*this, &Diffuse::notebooktab_button_press_cb),
                 viewer.get()));
  notebook.append_page(*viewer.get(), tab);
  notebook.set_tab_reorderable(*viewer.get(), true);
  tab.show();
  viewer->show();
  notebook.set_show_tabs(prefs.getBool("tabs_always_show") ||
                         (1 < notebook.get_n_pages()));
  viewer->signal_status_changed().connect(
      sigc::mem_fun(*this, &Diffuse::status_changed_cb));
  viewer->signal_syntax_changed().connect(
      sigc::mem_fun(*this, &Diffuse::syntax_changed_cb));
  viewer->signal_title_changed().connect(
      sigc::mem_fun(*this, &Diffuse::title_changed_cb));

  return viewer;
}

/**
 * @brief Create a new viewer to display 'items'
 * @param items
 */
void Df::Diffuse::newLoadedFileDiffViewer(const LabelledSpecs &items) {}

/**
 * @brief Callback used when a mouse button is pressed on a notebook tab
 * @param event
 * @param viewer
 * @return
 */
bool Df::Diffuse::notebooktab_button_press_cb(GdkEventButton *event,
                                              FileDiffViewer *viewer) {
  return false;
}

/**
 * @brief Notify all viewers of changes to the preferences
 */
void Df::Diffuse::preferences_updated() {}

/**
 * @brief Callback for the close button on each tab
 * @param viewer
 */
void Df::Diffuse::remove_tab_cb(FileDiffViewer *const viewer) {}

/**
 * @brief Save state information that should persist across sessions
 * @param[in] statepath Path to the state information file
 * @return _true_ on success, _false_ otherwise
 */
bool Df::Diffuse::saveState(const std::string &statepath) {
  std::vector<Glib::ustring> ss;

  for (const auto &s : bool_state) {
    ss.emplace_back(Glib::ustring::compose("%1 %2\n", s.first, s.second));
  }
  for (const auto &s : int_state) {
    ss.emplace_back(Glib::ustring::compose("%1 %2\n", s.first, s.second));
  }
  std::sort(std::begin(ss), std::end(ss));
  ss.emplace(
      std::cbegin(ss),
      Glib::ustring::compose("# This state file was generated by %1 %2.\n\n",
                             APP_NAME, VERSION));

  std::ofstream f{statepath, std::ios::out | std::ios::trunc};
  if (f.fail()) {
    std::cerr << "Failed to open state information file for writing\n";
    return false;
  }
  for (const auto &s : ss) {
    f << s;
    if (f.fail()) {
      std::cerr << "Failed to write to state information file\n";
      return false;
    }
  }
  f.close();
  if (!f.fail()) {
    return true;
  }

  std::cerr << "Failed to close state information file\n";
  return false;
}

/**
 * @brief Callback used when a viewer's status changes
 */
void Df::Diffuse::status_changed_cb() {}

/**
 * @brief Callback used when a viewer's syntax changes
 */
void Df::Diffuse::syntax_changed_cb() {}

/**
 * @brief Callback used when a viewer's title changes
 */
void Df::Diffuse::title_changed_cb() {}

/**
 * @brief Callback used when switching notebook pages
 * @param[in] ptr Pointer to the new current page
 * @param[in] page_num The index of the page
 */
void switch_page_cb(Gtk::Widget *const ptr, const guint page_num) {
  // TODO
}

/**
 * @brief Record the window's maximised state
 * @param[in] event The GdkEventWindowState triggered by a top-level window
 *   state change
 * @return _false_ in any case
 */
bool Df::Diffuse::window_state_cb(const GdkEventWindowState *const event) {
  bool_state["window_maximized"] =
      0 != (event->new_window_state & Gdk::WindowState::WINDOW_STATE_MAXIMIZED);

  return false;
}

Df::Diffuse::FileDiffViewer::FileDiffViewer(const guint n, Preferences &prefs,
                                            const Glib::ustring &title)
    : Df::FileDiffViewer{n, prefs} {}

Df::Diffuse::FileDiffViewer::type_signal_status_changed
Df::Diffuse::FileDiffViewer::signal_status_changed() {
  return m_status_changed;
}

Df::Diffuse::FileDiffViewer::type_signal_syntax_changed
Df::Diffuse::FileDiffViewer::signal_syntax_changed() {
  return m_syntax_changed;
}

Df::Diffuse::FileDiffViewer::type_signal_title_changed
Df::Diffuse::FileDiffViewer::signal_title_changed() {
  return m_title_changed;
}

/**
 * @brief Assign user specified labels to the corresponding files
 * @param items
 * @param labels
 */
static Df::Diffuse::LabelledSpecs
assign_file_labels(const Df::Diffuse::Specs &items,
                   const Df::Diffuse::Labels &labels) {
  Df::Diffuse::LabelledSpecs new_items;
  Df::Diffuse::Labels ss{labels.crbegin(), labels.crend()};
  for (const auto &item : items) {
    std::optional<Glib::ustring> s;
    if (!ss.empty()) {
      s = ss.back();
      ss.pop_back();
    }
    new_items.emplace_back(item, s);
  }
  return new_items;
}

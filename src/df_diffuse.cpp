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
#include <gtkmm/stock.h>

#include <gdkmm/general.h>

#include <glibmm/convert.h>
#include <glibmm/miscutils.h>

namespace Df = Diffuse;

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
}

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

bool Df::Diffuse::loadState(const std::string &statepath) { return true; }

void Df::Diffuse::newLoadedFileDiffViewer(const std::vector<void *> &items) {}

/**
 * @brief Notify all viewers to changes to the preferences
 */
void Df::Diffuse::preferences_updated() {}

void Df::Diffuse::saveState(const std::string &statepath) {}

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

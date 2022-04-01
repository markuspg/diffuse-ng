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

#include "df_scrolled_window.h"

namespace Df = Diffuse;

Df::ScrolledWindow::ScrolledWindow(Gtk::Adjustment &hdj, Gtk::Adjustment &vdj)
    // TODO: Fix memory leak
    : Gtk::Table{2u, 2u}, hadj{hdj}, vadj{vdj}, vport{*new Gtk::Adjustment{
                                                          0.0, 0.0, 0.0},
                                                      *new Gtk::Adjustment{
                                                          0.0, 0.0, 0.0}},
      hbar{hadj}, vbar{vadj} {
  // TODO: Replace darea's queue_draw_area with our own so we can tell when to
  // disable/enable our scrolling optimization
  // self.darea_queue_draw_area = darea.queue_draw_area
  // darea.queue_draw_area = self.redraw_region
  vport.add(darea);
  darea.show();
  attach(vport, 0u, 1u, 0u, 1u);
  vport.show();

  attach(vbar, 1u, 2u, 0u, 1u, Gtk::AttachOptions::FILL);
  vbar.show();
  attach(hbar, 0u, 1u, 1u, 2u,
         Gtk::AttachOptions::EXPAND | Gtk::AttachOptions::FILL,
         Gtk::AttachOptions::FILL);
  hbar.show();

  hadj.signal_value_changed().connect(
      sigc::mem_fun(*this, &ScrolledWindow::value_changed_cb));
  vadj.signal_value_changed().connect(
      sigc::mem_fun(*this, &ScrolledWindow::value_changed_cb));
  darea.signal_configure_event().connect(
      sigc::mem_fun(*this, &ScrolledWindow::configure_cb));
  darea.signal_scroll_event().connect(
      sigc::mem_fun(*this, &ScrolledWindow::scroll_cb));
  darea.signal_expose_event().connect(
      sigc::mem_fun(*this, &ScrolledWindow::expose_cb));
}

/**
 * @brief Updates the adjustments to match the new widget size
 * @param[in] event
 * @return
 */
bool Df::ScrolledWindow::configure_cb(const GdkEventConfigure *event) {
  return false;
}

bool Df::ScrolledWindow::expose_cb([
    [maybe_unused]] const GdkEventExpose *event) {
  scroll_count = 0u;

  return false;
}

/**
 * @brief Update the vertical adjustment when the mouse's scroll wheel is used
 * @param[in] event
 * @return
 */
bool Df::ScrolledWindow::scroll_cb(const GdkEventScroll *event) {
  return false;
}

void Df::ScrolledWindow::value_changed_cb() {}

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
#include "df_utils.h"

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
  const auto h{event->height};
  const auto w{event->width};

  for (auto &item :
       std::vector<std::pair<Gtk::Adjustment &, int>>{{hadj, w}, {vadj, h}}) {
    auto &adj{item.first};
    auto d{item.second};

    const auto v{adj.get_value()};
    if ((v + d) > adj.get_upper()) {
      adj.set_value(std::max(0.0, adj.get_upper() - d));
    }
    adj.set_page_size(d);
    adj.set_page_increment(d);
  }

  return false;
}

bool Df::ScrolledWindow::expose_cb([
    [maybe_unused]] const GdkEventExpose *event) {
  scroll_count = 0u;

  return false;
}

/**
 * @brief Replacement for darea.queue_draw_area that notifies when a partial
 *   redraw happened
 * @param x_y
 * @param w_h
 */
void Df::ScrolledWindow::redraw_region(const Position &x_y,
                                       const Position &w_h) {
  partial_redraw = true;
  // darea_queue_draw_area(x_y.x, x_y.y, w_h.w, w_h.h);
}

/**
 * @brief Update the vertical adjustment when the mouse's scroll wheel is used
 * @param[in] event
 * @return
 */
bool Df::ScrolledWindow::scroll_cb(const GdkEventScroll *event) {
  const auto d{event->direction};

  if (scroll_directions.cend() !=
      std::find(scroll_directions.cbegin(), scroll_directions.cend(), d)) {
    auto delta{100.0};

    if ((GDK_SCROLL_UP == d) || (GDK_SCROLL_LEFT == d)) {
      delta = -delta;
    }
    auto vertical{((GDK_SCROLL_UP == d) || (GDK_SCROLL_DOWN == d))};

    if (Gdk::SHIFT_MASK & event->state) {
      vertical = !vertical;
    }

    Gtk::Adjustment *adj = nullptr;
    if (vertical) {
      adj = &vadj;
    } else {
      adj = &hadj;
    }
    step_adjustment(*adj, delta);
  }

  return false;
}

void Df::ScrolledWindow::value_changed_cb() {
  const auto old_x_y{position};
  const auto pos_x{hadj.get_value()};
  const auto pos_y{vadj.get_value()};
  position = {pos_x, pos_y};
  // TODO:
  // if self.darea.window is not None:
  //     # window.scroll() although visually nice, is slow, revert to
  //     # queue_draw() if scroll a lot without seeing an expose event
  //     if self.scroll_count < 2 and not self.partial_redraw:
  //         self.scroll_count += 1
  //         self.darea.window.scroll(old_x - pos_x, old_y - pos_y)
  //     else:
  //         self.partial_redraw = False
  //         self.darea.queue_draw()
}

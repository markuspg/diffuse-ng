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

#ifndef DF_SCROLLED_WINDOW_H
#define DF_SCROLLED_WINDOW_H

#include <gtkmm/adjustment.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/table.h>
#include <gtkmm/viewport.h>

namespace Diffuse {
/**
 * @brief Replacement for Gtk::ScrolledWindow allowing to postpone expose events
 *
 * This is a replacement for Gtk::ScrolledWindow as it forced expose events to
 * be handled immediately after changing the viewport position. This could cause
 * the application to become unresponsive for a while as it processed a large
 * queue of keypress and expose event pairs.
 */
class ScrolledWindow : public Gtk::Table {
public:
  ScrolledWindow(Gtk::Adjustment &hadj, Gtk::Adjustment &vadj);

private:
  struct Position {
    double x;
    double y;
  };

  bool configure_cb(const GdkEventConfigure *event);
  bool expose_cb(const GdkEventExpose *event);
  void redraw_region(const Position &x_y, const Position &w_h);
  bool scroll_cb(const GdkEventScroll *event);
  void value_changed_cb();

  Gtk::DrawingArea darea;
  Gtk::Adjustment &hadj;
  bool partial_redraw{false};
  Position position{0.0, 0.0};
  unsigned scroll_count{0u};
  static constexpr std::array<GdkScrollDirection, 4> scroll_directions{
      GDK_SCROLL_UP, GDK_SCROLL_DOWN, GDK_SCROLL_LEFT, GDK_SCROLL_RIGHT};
  Gtk::Adjustment &vadj;
  Gtk::Viewport vport;
  Gtk::HScrollbar hbar;
  Gtk::VScrollbar vbar;
};
} // namespace Diffuse

#endif // DF_SCROLLED_WINDOW_H

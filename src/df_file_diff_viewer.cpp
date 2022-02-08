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

#include "df_file_diff_viewer.h"

namespace Df = Diffuse;

Df::FileDiffViewer::FileDiffViewer(std::size_t n, Preferences &prfs)
    : Gtk::Table{3, static_cast<guint>(n + 1)}, prefs{prfs} {
  // Verify the validity of the number of panes
  if (2 > n) {
    throw std::out_of_range{"Invalid number of panes"};
  }

  set_flags(Gtk::CAN_FOCUS);
}

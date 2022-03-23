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

#include "df_file_diff_viewer.h"

namespace Df = Diffuse;

Df::FileDiffViewer::FileDiffViewer(guint n, Preferences &prefs)
    : Gtk::Table{3, n + 1} {}

/**
 * @brief Returns True if there are any differences
 * @return _true_ if there are any differences, _false_ otherwise
 */
bool Df::FileDiffViewer::hasDifferences() const { return false; }

Df::FileDiffViewer::Line::Line(std::optional<unsigned> &line_no,
                               std::optional<Glib::ustring> &txt)
    : line_number{line_no}, text{txt} {}

std::optional<Glib::ustring> Df::FileDiffViewer::Line::getText() const {
  if (is_modified) {
    return modified_text;
  }

  return text;
}

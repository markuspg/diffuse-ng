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

#include "df_file_info.h"

namespace Df = Diffuse;

Df::FileInfo::FileInfo(const std::optional<Glib::ustring> &nm,
                       const std::optional<Glib::ustring> &encodng,
                       const std::optional<void *> &v_c_s,
                       const std::optional<Glib::ustring> &rev,
                       const std::optional<Glib::ustring> &lbl)
    : encoding{encodng}, label{lbl}, name{nm}, revision{rev}, vcs{v_c_s} {}

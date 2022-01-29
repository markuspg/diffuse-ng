/* Diffuse - a graphical tool for merging and comparing text files
 * Copyright (C) 2022 Markus Prasser
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef DF_DIFFUSE_H
#define DF_DIFFUSE_H

#include <gtkmm/window.h>

namespace Diffuse {
class Diffuse {
public:
  Diffuse(const Glib::ustring &rc_dir) {}

  bool loadState(const Glib::ustring &statepath) { return true; }
};
} // namespace Diffuse

#endif // DF_DIFFUSE_H

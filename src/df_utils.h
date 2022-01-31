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

#ifndef DF_UTILS_H
#define DF_UTILS_H

#include <glibmm/ustring.h>

#include <vector>

namespace Diffuse {
void logError(const Glib::ustring &s);
bool make_subdirs(Glib::ustring &p, const std::vector<Glib::ustring> &ss);
void printMessage(const Glib::ustring &s);
} // namespace Diffuse

#endif // DF_UTILS_H

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

#ifndef DF_FILE_INFO_H
#define DF_FILE_INFO_H

#include <glibmm/ustring.h>

#include <optional>

namespace Diffuse {
/**
 * @brief Contains information about a file
 */
class FileInfo {
public:
  FileInfo(const std::optional<Glib::ustring> &nm = std::nullopt,
           const std::optional<Glib::ustring> &encodng = std::nullopt,
           const std::optional<void *> &v_c_s = std::nullopt,
           const std::optional<Glib::ustring> &rev = std::nullopt,
           const std::optional<Glib::ustring> &lbl = std::nullopt);

  const std::optional<Glib::ustring> &getLabel() const { return label; }
  const std::optional<Glib::ustring> &getName() const { return name; }
  const std::optional<Glib::ustring> &getRevision() const { return revision; }

private:
  //! Name of codec used to translate the file contents to unicode text
  std::optional<Glib::ustring> encoding;
  // Alternate text to display instead of the actual file name
  std::optional<Glib::ustring> label;
  //! Most recent "stat" for files read from disk -- used on focus change to
  //! warn about changes to file on disk
  void *last_state = nullptr;
  //! File name
  std::optional<Glib::ustring> name;
  //! Revision used to retrieve file from the VCS
  std::optional<Glib::ustring> revision;
  //! "stat" for files read from disk -- used to warn about changes to the
  //! file on disk before saving
  void *stat = nullptr;
  //! The VCS object
  std::optional<void *> vcs = nullptr;
};
} // namespace Diffuse

#endif // DF_FILE_INFO_H

/*
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2021      Markus Prasser <markuspg@gmx.de>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the licence, or (at your option) any later
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

#ifndef DIFFUSE_FILEINFO_H_
#define DIFFUSE_FILEINFO_H_

#include <optional>

namespace Diffuse {
/*!
 * \brief Contains information about a file
 */
class FileInfo {
public:
  FileInfo(std::optional<void *> name = std::nullopt,
           std::optional<void *> encoding = std::nullopt,
           std::optional<void *> vcs = std::nullopt,
           std::optional<void *> revision = std::nullopt,
           std::optional<void *> label = std::nullopt);

private:
  //! Name of codec used to translate the file contents to Unicode text
  const std::optional<void *> file_encoding;
  //! Alternate text to display instead of the actual file name
  const std::optional<void *> file_label;
  //! Most recent "stat" for files read from disk -- used on focus change to
  //! warn about changes to file on disk
  const std::optional<void *> file_last_stat;
  //! File name
  const std::optional<void *> file_name;
  //! Revision used to retrieve file from VCS
  const std::optional<void *> file_revision;
  //! "stat" for files read from disk -- used to warn about changes to the file
  //! on disk before saving
  const std::optional<void *> file_stat;
  //! The VCS object
  const std::optional<void *> file_vcs;
};
} // namespace Diffuse

#endif // DIFFUSE_FILEINFO_H_

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

#ifndef DF_FILE_INFO_H
#define DF_FILE_INFO_H

#include "df_vcss.h"

#include <glibmm/ustring.h>

namespace Diffuse {
/**
 * @brief Contains information about a file
 */
class FileInfo {
public:
  FileInfo(void *name = nullptr, void *encoding = nullptr, void *vcs = nullptr,
           void *revision = nullptr, void *label = nullptr);

private:
  //! Name of codec used to translate the file contents to unicode text
  void *encoding = nullptr;
  //! Alternate text to display instead of the actual file name
  void *label = nullptr;
  //! Most recent "stat" for files read from disk -- used on focus change to
  //! warn about changes to file on disk
  void *last_stat = nullptr;
  //! File name
  void *name = nullptr;
  //! Revision used to retrieve file from the VCS
  void *revision = nullptr;
  //! "stat" for files read from disk -- used to warn about changes to the file
  //! on disk before saving
  void *stat = nullptr;
  //! The VCS object
  void *vcs = nullptr;
};
} // namespace Diffuse

#endif // DF_FILE_INFO_H

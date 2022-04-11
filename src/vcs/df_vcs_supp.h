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

#ifndef DF_VCS_SUPP_H
#define DF_VCS_SUPP_H

#include "../df_preferences.h"

#include <string>

namespace Diffuse {
/**
 * @brief Class representing base functionality to be supported by all version
 *   control systems
 *
 * Each class deriving from this should implement:
 *
 * * a constructor taking the path to the root folder of the repository
 * * getCommitTemplate
 * * getFileTemplate
 * * getFolderTemplate
 * * getRevision
 */
class VcsSupp {
public:
  using FileTemplate =
      std::vector<std::pair<std::string, std::optional<Glib::ustring>>>;
  /**
   * @brief VcsSupp
   * @param[in] root The path to the root folder of the repository
   */
  explicit VcsSupp(const std::string &root);
  virtual ~VcsSupp();

  /**
   * @brief Indicate which revisions to display for a commit
   */
  virtual void getCommitTemplate() = 0;
  /**
   * @brief Indicate which revisions to display for a file when none were
   *   requested explicitly
   */
  virtual FileTemplate getFileTemplate(const Preferences &prefs,
                                       const std::string &name) = 0;
  /**
   * @brief Indicate which file revisions to display for a set of folders
   */
  virtual void getFolderTemplate() = 0;
  /**
   * @brief Return the contents of the specified file revision
   */
  virtual void getRevision() = 0;

private:
  //! The repository's root folder
  const std::string root;
};
} // namespace Diffuse

#endif // DF_VCS_SUPP_H

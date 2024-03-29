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

#include "df_vcss.h"
#include "vcs/df_bzr.h"
#include "vcs/df_cvs.h"
#include "vcs/df_darcs.h"
#include "vcs/df_hg.h"
#include "vcs/df_mtn.h"
#include "vcs/df_svn.h"

#include <glibmm/fileutils.h>
#include <glibmm/miscutils.h>
#include <glibmm/regex.h>

namespace Df = Diffuse;

namespace Diffuse {

class VcsSupp;

static std::optional<std::string>
find_parent_dir_with(const std::string &path, const std::string &dir_name);
static std::unique_ptr<VcsSupp> get_bzr_repo(const std::string &path,
                                             const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_cvs_repo(const std::string &path,
                                             const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_darcs_repo(const std::string &path,
                                               const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_git_repo(const std::string &path,
                                             const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_hg_repo(const std::string &path,
                                            const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_mtn_repo(const std::string &path,
                                             const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_rcs_repo(const std::string &path,
                                             const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_svk_repo(const std::string &path,
                                             const Df::Preferences &prefs);
static std::unique_ptr<VcsSupp> get_svn_repo(const std::string &path,
                                             const Df::Preferences &prefs);

} // namespace Diffuse

Df::VCSs::VCSs()
    : // Initialize the VCS objects
      get_repo{{"bzr", &get_bzr_repo},     {"cvs", &get_cvs_repo},
               {"darcs", &get_darcs_repo}, {"git", &get_git_repo},
               {"hg", &get_hg_repo},       {"mtn", &get_mtn_repo},
               {"rcs", &get_rcs_repo},     {"svk", &get_svk_repo},
               {"svn", &get_svn_repo}} {}

/**
 * @brief Determines which VCS to use for the named file
 * @param path
 * @param prefs
 */
void Df::VCSs::findByFilename(const std::optional<std::string> &name,
                              Preferences &prefs) {
  if (name) {
    findByFolder(Glib::path_get_dirname(name.value()), prefs);
  }
}

/**
 * @brief Determines which VCS to use for files in the named folder
 * @param path
 * @param prefs
 */
void Df::VCSs::findByFolder(const std::string &path, Preferences &prefs) {
  auto tmp = g_canonicalize_filename(path.c_str(), nullptr);
  const std::string tmp_path{tmp};
  g_free(tmp);
  tmp = nullptr;

  const auto preWhitespaceRegexp{Glib::Regex::create("^\\s*")};
  const auto postWhitespaceRegexp{Glib::Regex::create("\\s*$")};

  auto tmp_str{preWhitespaceRegexp->replace(prefs.getString("vcs_search_order"),
                                            0, "", Glib::REGEX_MATCH_NOTEMPTY)};
  tmp_str =
      postWhitespaceRegexp->replace(tmp_str, 0, "", Glib::REGEX_MATCH_NOTEMPTY);

  const std::vector<Glib::ustring> vcs_search_order{
      Glib::Regex::split_simple("\\s+", tmp_str)};

  for (const auto &vcs : vcs_search_order) {
    if (get_repo.end() != get_repo.find(vcs)) {
    }
  }
}

/**
 * @brief Utility method to help find folders used by version control systems
 * @param path
 * @param dir_name
 * @return
 */
static std::optional<std::string>
Df::find_parent_dir_with(const std::string &path, const std::string &dir_name) {
  auto tmp_path{path};
  while (true) {
    const auto name{Glib::build_filename(tmp_path, dir_name)};
    if (Glib::file_test(name, Glib::FileTest::FILE_TEST_IS_DIR)) {
      return tmp_path;
    }
    auto newpath{Glib::path_get_dirname(tmp_path)};
    if (newpath == tmp_path) {
      break;
    }
    tmp_path = newpath;
  }

  return std::nullopt;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_bzr_repo(const std::string &path,
                 [[maybe_unused]] const Df::Preferences &prefs) {
  const auto p{find_parent_dir_with(path, "bzr")};
  if (p) {
    return std::make_unique<Df::Bzr>(p.value());
  }

  return nullptr;
}
static std::unique_ptr<Df::VcsSupp>
Df::get_cvs_repo(const std::string &path,
                 [[maybe_unused]] const Df::Preferences &prefs) {
  const auto p{find_parent_dir_with(path, "CVS")};
  if (p) {
    return std::make_unique<Df::Cvs>(p.value());
  }

  return nullptr;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_darcs_repo(const std::string &path,
                   [[maybe_unused]] const Df::Preferences &prefs) {
  const auto p{find_parent_dir_with(path, "_darcs")};
  if (p) {
    return std::make_unique<Df::Darcs>(p.value());
  }

  return nullptr;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_git_repo(const std::string &path, const Df::Preferences &prefs) {
  return nullptr;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_hg_repo(const std::string &path,
                [[maybe_unused]] const Df::Preferences &prefs) {
  const auto p{find_parent_dir_with(path, ".hg")};
  if (p) {
    return std::make_unique<Df::Hg>(p.value());
  }

  return nullptr;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_mtn_repo(const std::string &path,
                 [[maybe_unused]] const Df::Preferences &prefs) {
  const auto p{find_parent_dir_with(path, "_MTN")};
  if (p) {
    return std::make_unique<Df::Mtn>(p.value());
  }

  return nullptr;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_rcs_repo(const std::string &path, const Df::Preferences &prefs) {
  return nullptr;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_svk_repo(const std::string &path, const Df::Preferences &prefs) {
  return nullptr;
}

static std::unique_ptr<Df::VcsSupp>
Df::get_svn_repo(const std::string &path,
                 [[maybe_unused]] const Df::Preferences &prefs) {
  const auto p{find_parent_dir_with(path, ".svn")};
  if (p) {
    return std::make_unique<Df::Svn>(p.value());
  }

  return nullptr;
}

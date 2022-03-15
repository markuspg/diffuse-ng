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
  virtual void getFileTemplate() = 0;
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

/**
 * @brief Bazaar support
 */
class Bzr : public VcsSupp {
public:
  explicit Bzr(const std::string &root);
  ~Bzr() override;

  void getCommitTemplate() override;
  void getFileTemplate() override;
  void getFolderTemplate() override;
  void getRevision() override;
};

/**
 * @brief CVS support
 */
class Cvs : public VcsSupp {
public:
  explicit Cvs(const std::string &root);
  ~Cvs() override;
};

/**
 * @brief Darcs support
 */
class Darcs : public VcsSupp {
public:
  explicit Darcs(const std::string &root);
  ~Darcs() override;
};

/**
 * @brief Git support
 */
class Git : public VcsSupp {
public:
  explicit Git(const std::string &root);
  ~Git() override;
};

/**
 * @brief Mercurial support
 */
class Hg : public VcsSupp {
public:
  explicit Hg(const std::string &root);
  ~Hg() override;

private:
  std::optional<void *> working_rev;
};

/**
 * @brief Monotone support
 */
class Mtn : public VcsSupp {
public:
  explicit Mtn(const std::string &root);
  ~Mtn() override;
};

/**
 * @brief RCS support
 */
class Rcs : public VcsSupp {
public:
  explicit Rcs(const std::string &root);
  ~Rcs() override;
};

/**
 * @brief Subversion support
 *
 * SVK support subclasses from this.
 */
class Svn : public VcsSupp {
public:
  explicit Svn(const std::string &root);
  ~Svn() override;

private:
  std::optional<void *> url;
};

class Svk : public Svn {
public:
  explicit Svk(const std::string &root);
  ~Svk() override;
};

} // namespace Diffuse

Df::VCSs::VCSs()
    : // Initialize the VCS objects
      get_repo{{"bzr", nullptr}, {"cvs", nullptr}, {"darcs", nullptr},
               {"git", nullptr}, {"hg", nullptr},  {"mtn", nullptr},
               {"rcs", nullptr}, {"svk", nullptr}, {"svn", nullptr}} {}

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

Df::Bzr::Bzr(const std::string &rt) : VcsSupp{rt} {}

Df::Bzr::~Bzr() {}

void Df::Bzr::getCommitTemplate() {}

void Df::Bzr::getFileTemplate() {}

void Df::Bzr::getFolderTemplate() {}

void Df::Bzr::getRevision() {}

Df::Cvs::Cvs(const std::string &rt) : VcsSupp{rt} {}

Df::Cvs::~Cvs() {}

Df::Darcs::Darcs(const std::string &rt) : VcsSupp{rt} {}

Df::Darcs::~Darcs() {}

Df::Git::Git(const std::string &rt) : VcsSupp{rt} {}

Df::Git::~Git() {}

Df::Hg::Hg(const std::string &rt) : VcsSupp{rt} {}

Df::Hg::~Hg() {}

Df::Mtn::Mtn(const std::string &rt) : VcsSupp{rt} {}

Df::Mtn::~Mtn() {}

Df::Rcs::Rcs(const std::string &rt) : VcsSupp{rt} {}

Df::Rcs::~Rcs() {}

Df::Svk::Svk(const std::string &rt) : Svn{rt} {}

Df::Svk::~Svk() {}

Df::Svn::Svn(const std::string &rt) : VcsSupp{rt} {}

Df::Svn::~Svn() {}

Df::VcsSupp::VcsSupp(const std::string &rt) : root{rt} {}

Df::VcsSupp::~VcsSupp() {}

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
Df::get_bzr_repo(const std::string &path, const Df::Preferences &prefs) {
  const auto p{find_parent_dir_with(path, "bzr")};
  if (p) {
    return std::make_unique<Df::Bzr>(p.value());
  }

  return nullptr;
}

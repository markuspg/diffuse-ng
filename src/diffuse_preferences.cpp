/*
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2020      Markus Prasser <markuspg@users.noreply.github.com>
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

#include "diffuse_preferences.h"
#include "diffuse_encodingmenu.h"
#include "diffuse_globals.h"
#include "diffuse_utils.h"

#include <boost/algorithm/string.hpp>

#include <glibmm/i18n.h>
#include <glibmm/regex.h>

namespace Df = Diffuse;

static std::vector<Glib::ustring> get_encodings();

Df::Preferences::Preferences(const Glib::ustring &path)
    : encodings{get_encodings()},
      disable_when{
          {"display_right_margin", {"display_show_right_margin", false}},
          {"display_ignore_whitespace_changes",
           {"display_ignore_whitespace", true}},
          {"display_ignore_blanklines", {"display_ignore_whitespace", true}},
          {"display_ignore_endofline", {"display_ignore_whitespace", true}},
          {"align_ignore_whitespace_changes",
           {"align_ignore_whitespace", true}},
          {"align_ignore_blanklines", {"align_ignore_whitespace", true}},
          {"align_ignore_endofline", {"align_ignore_whitespace", true}}},
      tmplt{
          {Category{
               _("Display"),
               {std::make_shared<FontOpt>("display_font", "Monospace 10",
                                          _("Font")),
                std::make_shared<IntegerOpt>("display_tab_width", 8,
                                             _("Tab width"), 1, 1024),
                std::make_shared<BoolOpt>("display_show_right_margin", true,
                                          _("Show right margin")),
                std::make_shared<IntegerOpt>("display_right_margin", 80,
                                             _("Right margin"), 1, 8192),
                std::make_shared<BoolOpt>("display_show_line_numbers", true,
                                          _("Show line numbers")),
                std::make_shared<BoolOpt>("display_show_whitespace", false,
                                          _("Show white space characters")),
                std::make_shared<BoolOpt>("display_ignore_case", false,
                                          _("Ignore case differences")),
                std::make_shared<BoolOpt>("display_ignore_whitespace", false,
                                          _("Ignore white space differences")),
                std::make_shared<BoolOpt>("display_ignore_whitespace_changes",
                                          false,
                                          _("Ignore changes to white space")),
                std::make_shared<BoolOpt>("display_ignore_blanklines", false,
                                          _("Ignore blank line differences")),
                std::make_shared<BoolOpt>(
                    "display_ignore_endofline", false,
                    _("Ignore end of line differences"))}},
           Category{
               _("Alignment"),
               {std::make_shared<BoolOpt>("align_ignore_case", false,
                                          _("Ignore case")),
                std::make_shared<BoolOpt>("align_ignore_whitespace", true,
                                          _("Ignore white space")),
                std::make_shared<BoolOpt>("align_ignore_whitespace_changes",
                                          false,
                                          _("Ignore changes to white space")),
                std::make_shared<BoolOpt>("align_ignore_blanklines", false,
                                          _("Ignore blank lines")),
                std::make_shared<BoolOpt>("align_ignore_endofline", true,
                                          _("Ignore end of line characters"))}},
           Category{
               _("Editor"),
               {std::make_shared<BoolOpt>("editor_auto_indent", true,
                                          _("Auto indent")),
                std::make_shared<BoolOpt>("editor_expand_tabs", false,
                                          _("Expand tabs to spaces")),
                std::make_shared<IntegerOpt>("editor_soft_tab_width", 8,
                                             _("Soft tab width"), 1, 1024)}},
           Category{_("Tabs"),
                    {std::make_shared<IntegerOpt>("tabs_default_panes", 2,
                                                  _("Default panes"), 2, 16),
                     std::make_shared<BoolOpt>("tabs_always_show", false,
                                               _("Always show the tab bar")),
                     std::make_shared<BoolOpt>(
                         "tabs_warn_before_quit", true,
                         Glib::ustring::compose(
                             _("Warn me when closing a tab will quit %1"),
                             APP_NAME))}},
           Category{
               _("Regional Settings"),
               {std::make_shared<EncodingOpt>(
                   "encoding_default_codec",
                   run(std::vector<Glib::ustring>{
                           "python2", "-c",
                           "import sys; print sys.getfilesystemencoding()"})
                       ->second,
                   _("Default codec"))}}}} {
  const Glib::ustring svk_bin{isWindows() ? "svk.bat" : "svk"};

  std::vector<Glib::ustring> auto_detect_codecs{"utf_8", "utf_16", "latin_1"};

  const auto file_sys_enc{
      run(std::vector<Glib::ustring>{"python2", "-c",
                                     "import sys; print "
                                     "sys.getfilesystemencoding()"})};
  if (file_sys_enc &&
      (std::find(auto_detect_codecs.cbegin(), auto_detect_codecs.cend(),
                 file_sys_enc->second) == auto_detect_codecs.cend())) {
    // Insert after UTF-8 as the default encoding may prevent UTF-8 from being
    // tried
    auto_detect_codecs.emplace(auto_detect_codecs.cbegin() + 1,
                               file_sys_enc->second);
  }
  tmplt.back().list.emplace_back(std::make_shared<StringOpt>(
      "encoding_auto_detect_codecs",
      boost::algorithm::join(auto_detect_codecs, " "),
      _("Order of codecs used to identify encoding")));

  if (isWindows()) {
    bool found = false;
    auto root = Glib::getenv("SYSTEMDRIVE", found);
    const auto root_size = root.size();
    if ((!found) || root.empty()) {
      root = "C:\\\\";
    } else if ((2 > root.size()) || ((root[root_size - 2] != '\\') &&
                                     (root[root_size - 1] != '\\'))) {
      root.append("\\\\");
    }
    tmplt.emplace_back(Category{
        _("Cygwin"),
        {std::make_shared<FileOpt>(
             Glib::ustring{"cygwin_root"},
             Glib::ustring{
                 Glib::build_path(G_DIR_SEPARATOR_S,
                                  std::vector<Glib::ustring>{root, "cygwin"})},
             Glib::ustring{_("Root directory")}),
         std::make_shared<StringOpt>("cygwin_cygdrive_prefix", "/cygdrive",
                                     _("Cygdrive prefix"))}});
  }

  struct VCSInfo {
    const Glib::ustring key;
    const Glib::ustring name;
    const boost::optional<Glib::ustring> cmd;
  };
  // Create template for Version Control options
  const std::vector<VCSInfo> vcs{{"bzr", "Bazaar", Glib::ustring{"bzr"}},
                                 {"cvs", "CVS", Glib::ustring{"cvs"}},
                                 {"darcs", "Darcs", Glib::ustring{"darcs"}},
                                 {"git", "Git", Glib::ustring{"git"}},
                                 {"hg", "Mercurial", Glib::ustring{"hg"}},
                                 {"mtn", "Monotone", Glib::ustring{"mtn"}},
                                 {"rcs", "RCS", boost::none},
                                 {"svn", "Subversion", Glib::ustring{"svn"}},
                                 {"svk", "SVK", svk_bin}};

  FolderSet vcs_template;
  for (const auto &vc : vcs) {
    Category tmp{vc.name, {}};
    if ("rcs" == vc.key) {
      // RCS uses multiple commands
      tmp.list.emplace_back(
          std::make_shared<FileOpt>(vc.key + "_bin_co", Glib::ustring{"co"},
                                    Glib::ustring{_("\"co\" command")}));
      tmp.list.emplace_back(
          std::make_shared<FileOpt>(vc.key + "_bin_rlog", Glib::ustring{"rlog"},
                                    Glib::ustring{_("\"rlog\" command")}));
    } else {
      tmp.list.emplace_back(std::make_shared<FileOpt>(
          vc.key + "_bin", vc.cmd, Glib::ustring{_("Command")}));
    }
    if (isWindows()) {
      tmp.list.emplace_back(std::make_shared<BoolOpt>(
          vc.key + "_bash", false, _("Launch from a Bash login shell")));
      if ("git" != vc.key) {
        tmp.list.emplace_back(std::make_shared<BoolOpt>(
            vc.key + "_cygwin", false, _("Update paths for Cygwin")));
      }
    }
    vcs_template.emplace_back(std::move(tmp));
  }
}

Glib::ustring Df::Preferences::convertToNativePath(const Glib::ustring &s) {
  return s;
}

bool Df::Preferences::getBool(const Glib::ustring &name) const {
  return bool_prefs.at(name);
}

Glib::ustring Df::Preferences::getDefaultEncoding() const {
  return string_prefs.at("encoding_default_codec");
}

std::vector<Glib::ustring> Df::Preferences::getDefaultEncodings() const {
  return std::vector<Glib::ustring>{
      Glib::Regex::split_simple(" ", "encoding_auto_detect_codecs")};
}

std::vector<Glib::ustring> Df::Preferences::getEncodings() const {
  return encodings;
}

int Df::Preferences::getInt(const Glib::ustring &name) const {
  return int_prefs.at(name);
}

Glib::ustring Df::Preferences::getString(const Glib::ustring &name) const {
  return string_prefs.at(name);
}

void Df::Preferences::setBool(const Glib::ustring &name, bool value) {
  bool_prefs[name] = value;
}

/*!
 * \brief Query encodings available through Python 2
 */
static std::vector<Glib::ustring> get_encodings() {
  auto res = Df::run(std::vector<Glib::ustring>{
      "python2", "-c",
      "import encodings; print "
      "sorted(set(encodings.aliases.aliases.values()))"});
  Glib::ustring encodings = res->second.substr(2, res->second.size() - 5);
  std::vector<Glib::ustring> split_encs{
      Glib::Regex::split_simple("', '", encodings)};

  return split_encs;
}

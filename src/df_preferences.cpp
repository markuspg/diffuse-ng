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

#include "df_preferences.h"
#include "df_font_button.h"
#include "df_globals.h"
#include "df_utils.h"

#include <glibmm/convert.h>

#include <glib/gi18n.h>

#include <cstring>
#include <iostream>

namespace Df = Diffuse;

static constexpr auto unknown_entry_exc_txt{
    "Unknown entry in configuration file"};
static constexpr auto unknown_entry_exc_txt_size{sizeof(unknown_entry_exc_txt)};

Df::Preferences::Preferences(const std::string &pth)
    : disable_when{{"display_right_margin",
                    {"display_show_right_margin", false}},
                   {"display_ignore_whitespace_changes",
                    {"display_ignore_whitespace", true}},
                   {"display_ignore_blanklines",
                    {"display_ignore_whitespace", true}},
                   {"display_ignore_endofline",
                    {"display_ignore_whitespace", true}},
                   {"align_ignore_whitespace_changes",
                    {"align_ignore_whitespace", true}},
                   {"align_ignore_blanklines",
                    {"align_ignore_whitespace", true}},
                   {"align_ignore_endofline",
                    {"align_ignore_whitespace", true}}},
      path{pth},
      templt{
          {_("Display"),
           {FontPreference{"display_font", "Monospace 10", _("Font")},
            IntPreference{"display_tab_width", 8, _("Tab width"), 1, 1024},
            BoolPreference{"display_show_right_margin", true,
                           _("Show right margin")},
            IntPreference{"display_right_margin", 80, _("Right margin"), 1,
                          8192},
            BoolPreference{"display_show_line_numbers", true,
                           _("Show line numbers")},
            BoolPreference{"display_show_whitespace", false,
                           _("Show white space characters")},
            BoolPreference{"display_ignore_case", false,
                           _("Ignore case differences")},
            BoolPreference{"display_ignore_whitespace", false,
                           _("Ignore white space differences")},
            BoolPreference{"display_ignore_whitespace_changes", false,
                           _("Ignore changes to white space")},
            BoolPreference{"display_ignore_blanklines", false,
                           _("Ignore blank line differences")},
            BoolPreference{"display_ignore_endofline", false,
                           _("Ignore end of line differences")}}},
          {_("Alignment"),
           {BoolPreference{"align_ignore_case", false, _("Ignore case")},
            BoolPreference{"align_ignore_whitespace", true,
                           _("Ignore white space")},
            BoolPreference{"align_ignore_whitespace_changes", false,
                           _("Ignore changes to white space")},
            BoolPreference{"align_ignore_blanklines", false,
                           _("Ignore blank lines")},
            BoolPreference{"align_ignore_endofline", true,
                           _("Ignore end of line characters")}}},
          {_("Editor"),
           {BoolPreference{"editor_auto_indent", true, _("Auto indent")},
            BoolPreference{"editor_expand_tabs", false,
                           _("Expand tabs to spaces")},
            IntPreference{"editor_soft_tab_width", 8, _("Soft tab width"), 1,
                          1024}}},
          {_("Tabs"),
           {IntPreference{"tabs_default_panes", 2, _("Default panes"), 2, 16},
            BoolPreference{"tabs_always_show", false,
                           _("Always show the tab bar")},
            BoolPreference{
                "tabs_warn_before_quit", true,
                Glib::ustring::compose(
                    _("Warn me when closing a tab will quit %1"), APP_NAME)}}},
          {_("Regional Settings"),
           {EncodingPreference{"encoding_default_codec",
                               "" /* TODO: sys.getfilesystemencoding() */,
                               _("Default codec")},
            StringPreference{
                "encoding_auto_detect_codecs",
                "" /* TODO: ' '.join(auto_detect_codecs) */,
                _("Order of codecs used to identify encoding")}}} /*,
{, {,
*/} {
  // Find available encodings
  // TODO: self.encodings = sorted(set(encodings.aliases.aliases.values()))

  const Glib::ustring svk_bin{isWindows() ? "svk.bat" : "svk"};

  auto codeset{g_get_codeset()};
  const auto e{norm_encoding(codeset)};
  g_free(codeset);
  codeset = nullptr;

  std::vector<Glib::ustring> auto_detect_codecs{"utf_8", "utf_16", "latin_1"};
  if (auto_detect_codecs.cend() ==
      std::find(auto_detect_codecs.cbegin(), auto_detect_codecs.cend(), e)) {
    // Insert after UTF-8 as the default encoding may prevent UTF-8 from
    // being tried
    auto_detect_codecs.emplace(auto_detect_codecs.cbegin() + 1, e);
  }

  if (isWindows()) {
    bool found;
    auto root{Glib::getenv("SYSTEMDRIVE", found)};
    if (!found || root.empty()) {
      root = "C:\\\\";
    } else if ((2 > root.size()) || ("\\\\" != root.substr(root.size() - 2))) {
      root += "\\\\";
    }
    templt.emplace_back(
        Category{_("Cygwin"),
                 {FilePreference{"cygwin_root",
                                 Glib::locale_to_utf8(
                                     Glib::build_filename(root, "cygwin")),
                                 _("Root directory")},
                  StringPreference{"cygwin_cygdrive_prefix", "/cygdrive",
                                   _("Cygdrive prefix")}}});
  }

  // Create template for Version Control options
  struct VcsInfo {
    const Glib::ustring shortName;
    const Glib::ustring fullName;
    const std::optional<Glib::ustring> executableName;
  };
  const std::vector<VcsInfo> vcs{
      {"bzr", "Bazaar", "bzr"},     {"cvs", "CVS", "cvs"},
      {"darcs", "Darcs", "darcs"},  {"git", "Git", "git"},
      {"hg", "Mercurial", "hg"},    {"mtn", "Monotone", "mtn"},
      {"rcs", "RCS", std::nullopt}, {"svn", "Subversion", "svn"},
      {"svk", "SVK", svk_bin}};
  PrefsVec vcs_template{StringPreference{
      "vcs_search_order", "bzr cvs darcs git hg mtn rcs svn svk",
      _("Version control system search order")}};
  VcsPreferences vcs_folders_template;
  for (const auto &vc : vcs) {
    std::vector<std::variant<BoolPreference, std::vector<FilePreference>>> temp;
    if ("rcs" == vc.shortName) {
      // RCS uses multiple commands
      temp.emplace_back(std::vector<FilePreference>{
          {vc.shortName + "_bin", "co", _("\"co\" command")},
          {vc.shortName + "_bin_rlog", "rlog", _("\"rlog\" command")}});
    } else {
      temp.emplace_back(std::vector<FilePreference>{
          {vc.shortName + "_bin", vc.executableName.value(), _("Command")}});
    }
    if (isWindows()) {
      temp.emplace_back(BoolPreference{vc.shortName + "_bash", false,
                                       _("Launch from a Bash login shell")});
      if ("git" != vc.shortName) {
        temp.emplace_back(BoolPreference{vc.shortName + "_cygwin", false,
                                         _("Update paths for Cygwin")});
      }
    }
  }
  vcs_template.emplace_back(std::move(vcs_folders_template));
  templt.emplace_back(Category{_("Version Control"), std::move(vcs_template)});

  initFromTemplate();
  default_bool_prefs = bool_prefs;
  default_int_prefs = int_prefs;
  default_string_prefs = string_prefs;

  // Load the user's preferences
  if (Glib::file_test(path, Glib::FileTest::FILE_TEST_IS_REGULAR)) {
    try {
      std::ifstream f{path};
      if (f.fail()) {
        std::cerr << "Failed to open the user's preferences file\n";
        return;
      }
      const auto ss{readconfiglines(f)};
      if (!ss.has_value()) {
        return;
      }
      f.close();

      auto j = 0u;
      for (const auto &line : ss.value()) {
        ++j;
        try {
          const auto chunks{shlex_split(line, true)};
          if (!chunks.empty()) {
            const auto p{chunks.front()};
            if ((2 == chunks.size()) &&
                (bool_prefs.cend() != bool_prefs.find(p))) {
              bool_prefs[p] = ("True" == chunks[1]);
            } else if ((2 == chunks.size()) &&
                       (int_prefs.cend() != int_prefs.find(p))) {
              int_prefs[p] = std::clamp(std::stoi(chunks[1]), int_prefs_min[p],
                                        int_prefs_max[p]);
            } else if ((2 == chunks.size()) &&
                       (string_prefs.cend() != string_prefs.find(p))) {
              string_prefs[p] = chunks[1];
            } else {
              throw std::runtime_error{unknown_entry_exc_txt};
            }
          }
        } catch (const std::exception &exc) {
          if (!std::strncmp(unknown_entry_exc_txt, exc.what(),
                            unknown_entry_exc_txt_size)) {
            throw;
          }
          logDebug(Glib::ustring::compose("Error processing line %1 of %2.", j,
                                          Glib::locale_to_utf8(path)));
        }
      }
    } catch (const std::exception &exc) {
      if (!std::strncmp(unknown_entry_exc_txt, exc.what(),
                        unknown_entry_exc_txt_size)) {
        throw;
      }
      logDebug(Glib::ustring::compose("Error reading %1.",
                                      Glib::locale_to_utf8(path)));
    }
  }
}

std::string Df::Preferences::convertToNativePath(const Glib::ustring &s) {
  return Glib::locale_from_utf8(s);
}

bool Df::Preferences::getBool(const Glib::ustring &name) {
  return bool_prefs.at(name);
}

int Df::Preferences::getInt(const Glib::ustring &name) {
  return int_prefs.at(name);
}

Glib::ustring Df::Preferences::getString(const Glib::ustring &name) {
  return string_prefs.at(name);
}

/**
 * @brief Traverse templt to discover the preferences and their default values
 */
void Df::Preferences::initFromTemplate() {
  for (const auto &category : templt) {
    for (const auto &pref : category.preferences) {
      if (std::holds_alternative<BoolPreference>(pref)) {
        const auto &bool_pref{std::get<BoolPreference>(pref)};
        bool_prefs[bool_pref.name] = bool_pref.dflt;
      } else if (std::holds_alternative<IntPreference>(pref)) {
        const auto &int_pref{std::get<IntPreference>(pref)};
        int_prefs[int_pref.name] = int_pref.dflt;
        int_prefs_min[int_pref.name] = int_pref.min;
        int_prefs_max[int_pref.name] = int_pref.max;
      } else if (std::holds_alternative<EncodingPreference>(pref)) {
        const auto &enc_pref{std::get<EncodingPreference>(pref)};
        string_prefs[enc_pref.name] = enc_pref.dflt;
      } else if (std::holds_alternative<FilePreference>(pref)) {
        const auto &file_pref{std::get<FilePreference>(pref)};
        string_prefs[file_pref.name] = file_pref.dflt;
      } else if (std::holds_alternative<FontPreference>(pref)) {
        const auto &font_pref{std::get<FontPreference>(pref)};
        string_prefs[font_pref.name] = font_pref.dflt;
      } else if (std::holds_alternative<StringPreference>(pref)) {
        const auto &str_pref{std::get<StringPreference>(pref)};
        string_prefs[str_pref.name] = str_pref.dflt;
      } else if (std::holds_alternative<VcsPreferences>(pref)) {
        initFromVCSsTemplate(std::get<VcsPreferences>(pref));
      } else {
        throw std::runtime_error{"Unhandled type in Preferences template"};
      }
    }
  }
}

void Df::Preferences::initFromVCSsTemplate(const VcsPreferences &vcs_prefs) {
  for (const auto &vcs_pref : vcs_prefs) {
    for (const auto &pref : vcs_pref.opts) {
      if (std::holds_alternative<BoolPreference>(pref)) {
        const auto &bool_pref{std::get<BoolPreference>(pref)};
        bool_prefs[bool_pref.name] = bool_pref.dflt;
      } else if (std::holds_alternative<std::vector<FilePreference>>(pref)) {
        const auto &file_prefs{std::get<std::vector<FilePreference>>(pref)};
        for (const auto &file_pref : file_prefs) {
          string_prefs[file_pref.name] = file_pref.dflt;
        }
      } else {
        throw std::runtime_error{"Unhandled type in VCS Preferences template"};
      }
    }
  }
}

void Df::Preferences::setBool(const Glib::ustring &name, const bool value) {
  bool_prefs[name] = value;
}

void Df::Preferences::setString(const Glib::ustring &name,
                                const Glib::ustring &value) {
  string_prefs[name] = value;
}

Df::Preferences::BoolPreference::BoolPreference(const Glib::ustring &name,
                                                const bool dflt,
                                                const Glib::ustring &label)
    : Preference{name, label}, dflt{dflt} {}

Df::Preferences::EncodingPreference::EncodingPreference(
    const Glib::ustring &name, const Glib::ustring &dflt,
    const Glib::ustring &label)
    : Preference{name, label}, dflt{dflt} {}

Df::Preferences::FilePreference::FilePreference(const Glib::ustring &name,
                                                const Glib::ustring &dflt,
                                                const Glib::ustring &label)
    : Preference{name, label}, dflt{dflt} {}

Df::Preferences::FontPreference::FontPreference(const Glib::ustring &name,
                                                const Glib::ustring &dflt,
                                                const Glib::ustring &label)
    : Preference{name, label}, dflt{dflt} {}

Df::Preferences::IntPreference::IntPreference(const Glib::ustring &name,
                                              const int dflt,
                                              const Glib::ustring &label,
                                              const int min, const int max)
    : Preference{name, label}, dflt{dflt}, min{min}, max{max} {}

Df::Preferences::StringPreference::StringPreference(const Glib::ustring &name,
                                                    const Glib::ustring &dflt,
                                                    const Glib::ustring &label)
    : Preference{name, label}, dflt{dflt} {}

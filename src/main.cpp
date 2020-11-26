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

#include "diffuse_globals.h"
#include "diffuse_resources.h"
#include "diffuse_utils.h"
#include "diffuse_vcss.h"
#include "diffuse_window.h"

#include <glibmm/convert.h>
#include <glibmm/i18n.h>
#include <glibmm/miscutils.h>
#include <gtkmm/window.h>

namespace Df = Diffuse;

static std::vector<Glib::ustring> getConvertedArgs(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  // Use the program's location as a starting place to search for supporting
  // files such as icon and help documentation
  const auto app_path = Glib::filename_to_utf8(argv[0]);
  const auto bin_dir = Glib::path_get_dirname(app_path);

  // Locale stuff

  std::vector<Glib::ustring> args{getConvertedArgs(argc, argv)};

  if ((2 == argc) && (("-v" == args[1]) || ("--version" == args[1]))) {
    Df::printMessage(Df::APP_NAME + " " + Df::VERSION + "\n" + Df::COPYRIGHT);
    return 0;
  }
  if ((2 == argc) &&
      (("-h" == args[1]) || ("-?" == args[1]) || ("--help" == args[1]))) {
    Df::printMessage(
        "Usage:\n"
        "    diffuse [ [OPTION...] [FILE...] ]...\n"
        "    diffuse ( -h | -? | --help | -v | --version )\n"
        "\n"
        "Diffuse is a graphical tool for merging and comparing text files.  "
        "Diffuse is\n"
        "able to compare an arbitrary number of files side-by-side and gives "
        "users the\n"
        "ability to manually adjust line matching and directly edit files.  "
        "Diffuse can\n"
        "also retrieve revisions of files from Bazaar, CVS, Darcs, Git, "
        "Mercurial,\n"
        "Monotone, RCS, Subversion, and SVK repositories for comparison and "
        "merging.\n"
        "\n"
        "Help Options:\n"
        "  ( -h | -? | --help )             Display this usage information\n"
        "  ( -v | --version )               Display version and copyright "
        "information\n"
        "\n"
        "Configuration Options:\n"
        "  --no-rcfile                      Do not read any resource files\n"
        "  --rcfile <file>                  Specify explicit resource file\n"
        "\n"
        "General Options:\n"
        "  ( -c | --commit ) <rev>          File revisions <rev-1> and <rev>\n"
        "  ( -D | --close-if-same )         Close all tabs with no "
        "differences\n"
        "  ( -e | --encoding ) <codec>      Use <codec> to read and write "
        "files\n"
        "  ( -L | --label ) <label>         Display <label> instead of the "
        "file name\n"
        "  ( -m | --modified )              Create a new tab for each modified "
        "file\n"
        "  ( -r | --revision ) <rev>        File revision <rev>\n"
        "  ( -s | --separate )              Create a new tab for each file\n"
        "  ( -t | --tab )                   Start a new tab\n"
        "  --line <line>                    Start with line <line> selected\n"
        "  --null-file                      Create a blank file comparison "
        "pane\n"
        "\n"
        "Display Options:\n"
        "  ( -b | --ignore-space-change )   Ignore changes to white space\n"
        "  ( -B | --ignore-blank-lines )    Ignore changes in blank lines\n"
        "  ( -E | --ignore-end-of-line )    Ignore end of line differences\n"
        "  ( -i | --ignore-case )           Ignore case differences\n"
        "  ( -w | --ignore-all-space )      Ignore white space differences\n");
    return 0;
  }

  // Associate our icon with all of our windows
  Gtk::Window::set_default_icon_name("diffuse");

  Df::Resources theResources;

  Df::VCSs theVCSs;

  // Process the command line arguments

  // Find the config directory and create it if it didn't exist
  bool found = false;
  auto rc_dir = Glib::getenv("XDG_CONFIG_HOME", found);
  std::vector<Glib::ustring> subdirs{"diffuse"};
  if ((false == found) || rc_dir.empty()) {
    rc_dir = Glib::get_home_dir();
    subdirs.emplace(subdirs.cbegin(), ".config");
  }
  if (false == Df::make_subdirs(rc_dir, subdirs)) {
    return 1;
  }

  // Find the local data directory and create it if it didn't exist
  found = false;
  auto data_dir = Glib::getenv("XDG_DATA_HOME", found);
  subdirs = {"diffuse"};
  if ((false == found) || data_dir.empty()) {
    data_dir = Glib::get_home_dir();
    subdirs.emplace(subdirs.cbegin(), ".share");
    subdirs.emplace(subdirs.cbegin(), ".local");
  }
  if (false == Df::make_subdirs(data_dir, subdirs)) {
    return 1;
  }

  // Load resource files
  int i = 1;
  std::vector<Glib::ustring> rc_files;
  if ((2 == argc) && ("--no-rcfile" == args[1])) {
    ++i;
  } else if ((3 == argc) && ("--rcfile" == args[1])) {
    ++i;
    rc_files.emplace_back(args[i]);
    ++i;
  } else {
    // Parse system wide then personal initialization files
    Glib::ustring sys_rc_file;
    if (Df::isWindows()) {
      sys_rc_file = Glib::build_path(
          G_DIR_SEPARATOR_S, std::vector<Glib::ustring>{bin_dir, "diffuserc"});
    } else {
      sys_rc_file = Glib::build_path(
          G_DIR_SEPARATOR_S,
          std::vector<Glib::ustring>{bin_dir, "../../etc/diffuserc"});
    }
    if (g_file_test(sys_rc_file.c_str(), G_FILE_TEST_IS_REGULAR)) {
      rc_files.emplace_back(sys_rc_file);
    }
    const auto user_rc_file = Glib::build_path(
        G_DIR_SEPARATOR_S, std::vector<Glib::ustring>{rc_dir, "diffuserc"});
    if (g_file_test(user_rc_file.c_str(), G_FILE_TEST_IS_REGULAR)) {
      rc_files.emplace_back(user_rc_file);
    }
  }

  for (const auto &rc_file : rc_files) {
    // Convert to absolute path so the location of any processing errors are
    // reported with normalised file names
    const auto parse_rc_file = Glib::canonicalize_filename(rc_file.raw());
    if (false == theResources.parse(parse_rc_file)) {
      Df::logError(
          Glib::ustring::compose(_("Error reading %1."), parse_rc_file));
    }
  }

  Df::Window diff{rc_dir};
  // Load state
  const auto statepath = Glib::build_path(
      G_DIR_SEPARATOR_S, std::vector<Glib::ustring>{data_dir, "state"});
  if (false == diff.loadState(statepath)) {
    return false;
  }

  return 0;
}

static std::vector<Glib::ustring> getConvertedArgs(int argc, char *argv[]) {
  std::vector<Glib::ustring> tmp_args;

  for (auto i = 0; i < argc; ++i) {
    tmp_args.emplace_back(argv[i]);
  }

  return tmp_args;
}

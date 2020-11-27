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
#include <gtkmm/main.h>
#include <gtkmm/notebook.h>

#include <boost/lexical_cast.hpp>

namespace Df = Diffuse;

static std::vector<Glib::ustring> getConvertedArgs(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  Gtk::Main main{argc, argv};

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
    subdirs.emplace(subdirs.cbegin(), "share");
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

  // Process remaining command line arguments
  bool close_on_same = false;
  boost::optional<Glib::ustring> encoding;
  std::map<Glib::ustring,
           void (Diffuse::Window::*)(const Df::Window::Specs &,
                                     const std::vector<Glib::ustring> &,
                                     const Df::Window::Options &)>
      funcs{{"modified", &Df::Window::createModifiedFileTabs},
            {"commit", &Df::Window::createCommitFileTabs},
            {"separate", &Df::Window::createSeparateTabs},
            {"single", &Df::Window::createSingleTab}};
  bool had_specs = false;
  std::vector<Glib::ustring> labels;
  Glib::ustring mode = "single";
  Df::Window::Options options;
  Df::Window::Revs revs;
  Df::Window::Specs specs;
  while (i < argc) {
    const auto arg = args[i];
    if ((false == arg.empty()) && ('-' == arg[0])) {
      if ((i + 1 < argc) && (("-c" == arg) || ("--commit" == arg))) {
        // Specified revision
        (diff.*funcs[mode])(specs, labels, options);
        ++i;
        const auto rev = args[i];
        labels.clear();
        options["commit"] = args[i];
        specs.clear();
        mode = "commit";
      } else if (("-D" == arg) || ("--close-if-same" == arg)) {
        close_on_same = true;
      } else if ((i + 1 < argc) && (("-e" == arg) || ("--encoding" == arg))) {
        ++i;
        encoding = args[i];
        // TODO: encoding = encodings.aliases.aliases.get(encoding, encoding) */
      } else if (("-m" == arg) || ("--modified" == arg)) {
        (diff.*funcs[mode])(specs, labels, options);
        labels.clear();
        options.clear();
        specs.clear();
        mode = "modified";
      } else if ((i + 1 < argc) && (("-r" == arg) || ("--revision" == arg))) {
        // Specified revision
        ++i;
        revs.emplace_back(args[i], encoding);
      } else if (("-s" == arg) || ("--separate" == arg)) {
        (diff.*funcs[mode])(specs, labels, options);
        labels.clear();
        options.clear();
        specs.clear();
        // Open items in separate tabs
        mode = "separate";
      } else if (("-t" == arg) || ("--tab" == arg)) {
        (diff.*funcs[mode])(specs, labels, options);
        labels.clear();
        options.clear();
        specs.clear();
        // Start a new tab
        mode = "single";
      } else if (("-b" == arg) || ("--ignore-space-change" == arg)) {
        diff.prefs.setBool("display_ignore_whitespace_changes", true);
        diff.prefs.setBool("align_ignore_whitespace_changes", true);
        diff.preferences_updated();
      } else if (("-B" == arg) || ("--ignore-blank-lines" == arg)) {
        diff.prefs.setBool("display_ignore_blanklines", true);
        diff.prefs.setBool("align_ignore_blanklines", true);
        diff.preferences_updated();
      } else if (("-E" == arg) || ("--ignore-end-of-line" == arg)) {
        diff.prefs.setBool("display_ignore_endofline", true);
        diff.prefs.setBool("align_ignore_endofline", true);
        diff.preferences_updated();
      } else if (("-i" == arg) || ("--ignore-case" == arg)) {
        diff.prefs.setBool("display_ignore_case", true);
        diff.prefs.setBool("align_ignore_case", true);
        diff.preferences_updated();
      } else if (("-w" == arg) || ("--ignore-all-space" == arg)) {
        diff.prefs.setBool("display_ignore_whitespace", true);
        diff.prefs.setBool("align_ignore_whitespace", true);
        diff.preferences_updated();
      } else if ((i + 1 < argc) && ("-L" == arg)) {
        ++i;
        labels.emplace_back(args[i]);
      } else if ((i + 1 < argc) && ("--line" == arg)) {
        ++i;
        try {
          const auto line = boost::lexical_cast<int>(args[i]);
          options["line"] = line;
        } catch (const boost::bad_lexical_cast &) {
          Df::logError(_("Error parsing line number."));
        }
      } else if ("--null-file" == arg) {
        // Add a blank file pane
        if (("single" == mode) || ("separate" == mode)) {
          if (revs.empty()) {
            revs.emplace_back(boost::none, encoding);
          }
          specs.emplace_back(boost::none, revs);
          revs.clear();
        }
        had_specs = true;
      } else {
        Df::logError(Glib::ustring::compose(
            _("Skipping unknown argument \"%1\"."), args[i]));
      }
    } else {
      boost::optional<Glib::ustring> filename =
          diff.prefs.convertToNativePath(args[i]);
      if ((("single" == mode) || ("separate" == mode)) &&
          g_file_test(filename->c_str(), G_FILE_TEST_IS_DIR)) {
        if (false == specs.empty()) {
          filename = Glib::build_path(
              G_DIR_SEPARATOR_S,
              std::vector<Glib::ustring>{
                  boost::get(filename),
                  Glib::path_get_basename(boost::get(specs.back().first))});
        } else {
          Df::logError(Glib::ustring::compose(
              _("Error processing argument \"%1\".  Directory not expected."),
              args[i]));
          filename.reset();
        }
      }
      if (filename) {
        if (revs.empty()) {
          revs.emplace_back(boost::none, encoding);
        }
        specs.emplace_back(filename, revs);
        revs.clear();
      }
      had_specs = true;
    }
    ++i;
  }

  if ((("modified" == mode) || ("commit" == mode)) && specs.empty()) {
    specs.emplace_back(Glib::get_current_dir(),
                       Df::Window::Revs{{boost::none, encoding}});
    had_specs = true;
  }
  (diff.*funcs[mode])(specs, labels, options);

  // Create a file diff viewer if the command line arguments haven't implicitly
  // created any
  if (false == had_specs) {
    diff.newLoadedFileDiffViewer(nullptr);
  } else if (close_on_same) {
    diff.closeOnSame();
  }

  const auto nb = diff.notebook;
  const auto n = nb->get_n_pages();
  if (n > 0) {
    nb->set_show_tabs(diff.prefs.getBool("tabs_always_show") || (n > 1));
    nb->get_nth_page(0)->grab_focus();
    diff.show();
    main.run();
    // Save state
    diff.saveState(statepath);
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

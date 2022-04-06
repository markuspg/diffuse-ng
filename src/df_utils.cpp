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

#include "df_utils.h"
#include "df_globals.h"
#include "df_message_dialog.h"

#include <glibmm/convert.h>

#include <iostream>

namespace Df = Diffuse;

bool Df::isWindows() { return false; } // TODO

/**
 * @brief Convenience function to print debug messages to stderr
 * @param[in] s The debug message to be printed
 */
void Df::logDebug(const Glib::ustring &s) {
  // std::cerr << Glib::locale_from_utf8(
  //     Glib::ustring::compose("%1: %2\n", Df::APP_NAME, s));
}

/**
 * @brief Report error messages to the user through a message dialog window
 * @param[in] s The error message to display to the user
 */
void Df::logError(const Glib::ustring &s) {
  MessageDialog m{Gtk::MessageType::MESSAGE_ERROR, s};
  m.run();
}

/**
 * @brief Map an encoding name to its standard form
 * @param[in] e The encoding identifier which shall be mapped
 * @return The given encoding mapped to its standard form
 */
Glib::ustring Df::norm_encoding(const Glib::ustring &e) {
  // TODO: 'e' could be optional
  static const auto regexp{Glib::Regex::create("-")};

  return regexp->replace(e, 0, "_", static_cast<Glib::RegexMatchFlags>(0))
      .lowercase();
}

/**
 * @brief Print a UTF-8 encoded string to stdout in the host's native encoding
 * @param[in] s The string which shall be printed
 */
void Df::printMessage(const Glib::ustring &s) {
  try {
    std::cout << Glib::locale_from_utf8(s) << "\n";
  } catch (const Glib::ConvertError &) {
  }
}

std::optional<std::vector<Glib::ustring>>
Df::readconfiglines(std::ifstream &fd) {
  if (fd.fail()) {
    std::cerr << "File input stream is in failed state\n";
    return std::nullopt;
  }

  std::ostringstream sstr;
  sstr << fd.rdbuf();
  auto file_content{Glib::locale_to_utf8(sstr.str())};

  if (fd.fail()) {
    std::cerr << "File input stream is in failed state after reading\n";
    return std::nullopt;
  }

  const auto cr_regex{Glib::Regex::create("\\r")};
  file_content = cr_regex->replace(file_content, 0, "",
                                   static_cast<Glib::RegexMatchFlags>(0));

  const auto lf_regex{Glib::Regex::create("\\n")};
  return lf_regex->split(file_content);
}

/**
 * @brief Split a string replicating the behaviour of shlex.split of Python 2
 * @param[in] s The string which shall be split
 * @param[in] allow_comments if comments shall be treated as such an be skipped
 */
std::vector<Glib::ustring> Df::shlex_split(const Glib::ustring &s,
                                           const bool allow_comments) {
  std::vector<Glib::ustring> r;
  Glib::ustring::size_type idx{0};
  const auto n{s.size()};

  while (n > idx) {
    // Skip whitespace
    while ((idx < n) && (Glib::ustring::npos != whitespace.find(s[idx]))) {
      ++idx;
    }

    // Skip comments
    if ((n == idx) || (allow_comments && ('#' == s[idx]))) {
      break;
    }

    Glib::ustring v;
    while ((n > idx) && (Glib::ustring::npos == whitespace.find(s[idx]))) {
      const auto c{s[idx]};
      if ('\\' == c) {
        // Parse escaped character
        ++idx;
        if (n > idx) {
          v.push_back(s[idx]);
          ++idx;
        }
      } else if ('\'' == c) {
        // Parse '-quoted string
        ++idx;
        const auto end{s.find('\'', idx)};
        if (Glib::ustring::npos == end) {
          v.append(Glib::ustring{s, idx});
          idx = n;
        } else {
          v.append(Glib::ustring{s, idx, end - idx});
          idx = end + 1;
        }
      } else if ('"' == c) {
        // Parse "-quoted string
        ++idx;
        while (n > idx) {
          if ('"' == s[idx]) {
            ++idx;
            break;
          }
          const auto end{s.find('"', idx)};
          auto end1{end};
          if (Glib::ustring::npos == end1) {
            end1 = n;
          }
          while (idx < end1) {
            auto tmp_b{Glib::ustring{s, idx, end1 - idx}.find('\\')};
            const auto b{tmp_b + idx};
            if (Glib::ustring::npos == tmp_b) {
              v.append(Glib::ustring{s, idx, end1 - idx});
              idx = end1;
              break;
            }
            v.append(Glib::ustring{s, idx, b - idx});
            idx = b + 1;
            if (n > idx) {
              v.append(Glib::ustring{s, idx, 1});
              ++idx;
            }
          }
        }
      } else {
        // Parse unescaped character
        v.push_back(s[idx]);
        ++idx;
      }
    }
    r.emplace_back(v);
  }

  return r;
}

/**
 * @brief Utility method to step advance an adjustment
 * @param adj
 * @param delta
 */
void Df::step_adjustment(Gtk::Adjustment &adj, const double delta) {
  auto v{adj.get_value() + delta};
  // Clamp to the allowed range
  v = std::max(v, adj.get_lower());
  v = std::min(v, adj.get_upper() - adj.get_page_size());
  adj.set_value(v);
}

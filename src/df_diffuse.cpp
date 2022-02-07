/* Diffuse - A graphical tool for merging and comparing text files
 *
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2022      Markus Prasser
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "df_diffuse.h"

#include <glibmm/convert.h>
#include <glibmm/miscutils.h>

namespace Df = Diffuse;

Df::Diffuse::Diffuse(const Glib::ustring &rc_dir)
    : prefs{Glib::build_filename(rc_dir, Glib::locale_from_utf8("prefs"))} {}

/**
 * @brief Close all tabs without differences
 */
void Df::Diffuse::closeOnSame() {}

void Df::Diffuse::createCommitFileTabs(const Specs &items, const Labels &labels,
                                       const Options &options) {}

void Df::Diffuse::createModifiedFileTabs(const Specs &items,
                                         const Labels &labels,
                                         const Options &options) {}

void Df::Diffuse::createSeparateTabs(const Specs &items, const Labels &labels,
                                     const Options &options) {}

void Df::Diffuse::createSingleTab(const Specs &items, const Labels &labels,
                                  const Options &options) {}

bool Df::Diffuse::loadState(const std::string &statepath) { return true; }

void Df::Diffuse::newLoadedFileDiffViewer(const std::vector<void *> &items) {}

/**
 * @brief Notify all viewers to changes to the preferences
 */
void Df::Diffuse::preferences_updated() {}

void Df::Diffuse::saveState(const std::string &statepath) {}

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

#include "df_about_dialog.h"
#include "df_globals.h"

namespace Df = Diffuse;

Df::AboutDialog::AboutDialog() {
  set_logo_icon_name("diffuse");
  set_program_name(APP_NAME);
  set_version(VERSION);
  set_comments(
      "Diffuse is a graphical tool for merging and comparing text files.");
  set_copyright(COPYRIGHT);
  set_website(WEBSITE);
  set_authors(std::vector<Glib::ustring>{
      "Derrick Moser <derrick_moser@yahoo.com>", "Markus Prasser"});
  set_translator_credits("translator-credits");
  set_license(Glib::ustring{APP_NAME} + " " + VERSION + "\n" + COPYRIGHT +
              "\n\n" +
              "This program is free software; you can redistribute it and/or "
              "modify it under the terms of the GNU General Public License as "
              "published by the Free Software Foundation; either version 2 of "
              "the licence, or (at your option) any later version.\n"
              "\n"
              "This program is distributed in the hope that it will be useful, "
              "but WITHOUT ANY WARRANTY; without even the implied warranty of "
              "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
              "GNU General Public License for more details.\n"
              "\n"
              "You should have received a copy of the GNU General Public "
              "License along with this program.  You may also obtain a copy of "
              "the GNU General Public License from the Free Software "
              "Foundation by visiting their web site (http://www.fsf.org/) or "
              "by writing to the Free Software Foundation, Inc., 51 Franklin "
              "St, Fifth Floor, Boston, MA  02110-1301  USA");
  set_wrap_license(true);
}

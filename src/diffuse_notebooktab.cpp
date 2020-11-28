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

#include "diffuse_notebooktab.h"

#include <glibmm/i18n.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/stock.h>

namespace Df = Diffuse;

Df::NotebookTab::NotebookTab(const Glib::ustring &name,
                             const boost::optional<Gtk::StockID> &stock) {
  set_visible_window(false);
  const auto hbox = manage(new Gtk::HBox);
  if (stock) {
    const auto image = manage(new Gtk::Image);
    gtk_image_set_from_stock(image->gobj(), stock->get_c_str(),
                             GTK_ICON_SIZE_MENU);
    hbox->pack_start(*image, false, false, 5);
    image->show();
  }
  label = manage(new Gtk::Label{name});
  // Left justify the widget
  label->set_alignment(0, 0.5);
  hbox->pack_start(*label, true, true, 0);
  label->show();
  button = manage(new Gtk::Button);
  button->set_relief(Gtk::RELIEF_NONE);
  const auto image = manage(new Gtk::Image);
  gtk_image_set_from_stock(image->gobj(), GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
  button->add(*image);
  image->show();
  button->set_tooltip_text(_("Close Tab"));
  hbox->pack_start(*button, false, false, 0);
  button->show();
  add(*hbox);
  hbox->show();
}

Glib::ustring Df::NotebookTab::get_text() const { return label->get_text(); }

void Df::NotebookTab::set_text(const Glib::ustring &s) { label->set_text(s); }

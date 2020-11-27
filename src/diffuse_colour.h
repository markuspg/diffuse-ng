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

#ifndef DIFFUSE_COLOUR_H
#define DIFFUSE_COLOUR_H

namespace Diffuse {
/*!
 * \brief Colour resources
 */
class Colour {
public:
  Colour(float r, float g, float b, float a = 1.0f);

  Colour operator+(const Colour &other) const;
  Colour operator*(float s) const;

  Colour over(const Colour &other) const;

private:
  // The individual colour components as floats in the range [0, 1]
  float alpha = 0.0f;
  float blue = 0.0f;
  float green = 0.0f;
  float red = 0.0f;
};
} // namespace Diffuse

#endif // DIFFUSE_COLOUR_H

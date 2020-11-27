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

#include "diffuse_colour.h"

#include <algorithm>

namespace Df = Diffuse;

static float frame(float f);

Df::Colour::Colour(float r, float g, float b, float a)
    : alpha{frame(a)}, blue{frame(b)}, green{frame(g)}, red{frame(r)} {}

/*!
 * \brief Add colours
 */
Df::Colour Df::Colour::operator+(const Colour &other) const {
  return {red + other.red, green + other.green, blue + other.blue,
          alpha + other.alpha};
}

/*!
 * \brief Multiply by scalar
 */
Df::Colour Df::Colour::operator*(const float s) const {
  return {red * s, green * s, blue * s, alpha * s};
}

Df::Colour Df::Colour::over(const Colour &other) const {
  return *this + (other * (1 - alpha));
}

static float frame(float f) { return std::max(std::min(f, 1.0f), 0.0f); }

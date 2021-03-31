/*
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2021      Markus Prasser <markuspg@gmx.de>
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

#ifndef DIFFUSE_COLOUR_H_
#define DIFFUSE_COLOUR_H_

namespace Diffuse {
constexpr float constrict(float f) noexcept {
  return f < 0.0f ? 0.0f : (f > 1.0f ? 1.0f : f);
}

/*!
 * \brief Colour resources
 */
class Colour {
public:
  constexpr Colour(float r, float g, float b, float a = 1.0f)
      : alpha{constrict(a)}, blue{constrict(b)}, green{constrict(g)},
        red{constrict(r)} {}

  /*!
   * \brief Add colours
   * \param[in] s The Colour instance to add to this one
   * \return The modified instance
   */
  Colour &operator+=(const Colour &s) noexcept {
    alpha = constrict(alpha + s.alpha);
    blue = constrict(blue + s.blue);
    green = constrict(green + s.green);
    red = constrict(red + s.red);

    return *this;
  }

  /*!
   * \brief Multiply by scalar
   * \param[in] s The scalar to multiply all colour components with
   * \return The modified instance
   */
  Colour &operator*=(float s) noexcept {
    alpha = constrict(alpha * s);
    blue = constrict(blue * s);
    green = constrict(green * s);
    red = constrict(red * s);

    return *this;
  }

  /*!
   * \brief Over operator
   */
  Colour over(const Colour &other) const noexcept {
    Colour tmp{other};
    tmp *= (1 - alpha);
    return Colour{*this} += tmp;
  }

private:
  // The individual colour components as floats in the range [0, 1]
  float alpha;
  float blue;
  float green;
  float red;
};
} // namespace Diffuse

#endif // DIFFUSE_COLOUR_H_

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

#ifndef DF_COLOUR_H
#define DF_COLOUR_H

namespace Diffuse {
class Colour;

constexpr Colour operator+(const Colour &lhs, const Colour &rhs);
constexpr Colour operator*(const Colour &lhs, float s);

inline constexpr float normalize_colour(float val) {
  return val > 1.0f ? 1.0f : (val < 0.0f ? 0.0f : val);
}

/**
 * @brief Class representing colours
 */
struct Colour {
  constexpr Colour(float r, float g, float b, float a = 1.0f)
      : alpha{normalize_colour(a)}, blue{normalize_colour(b)},
        green{normalize_colour(g)}, red{normalize_colour(r)} {}

  /**
   * @brief Over operator
   * @param other
   * @return
   */
  constexpr Colour over(const Colour &other) {
    return *this + other * (1 - alpha);
  }

  // The individual components are represented by floats in the range [0, 1]
  const float alpha;
  const float blue;
  const float green;
  const float red;
};

/**
 * @brief Add two Colour instances
 * @param[in] lhs The first summand
 * @param[in] rhs The second summand
 * @return The new resulting Colour instance
 */
constexpr Colour operator+(const Colour &lhs, const Colour &rhs) {
  return {lhs.red + rhs.red, lhs.green + rhs.green, lhs.blue + rhs.blue,
          lhs.alpha + rhs.alpha};
}

/**
 * @brief Multiply a Colour instance by a scalar
 * @param[in] lhs The Colour instance to be multiplied
 * @param[in] s The scalar to multiply the Colour instance with
 * @return The new resulting Colour instance
 */
constexpr Colour operator*(const Colour &lhs, const float s) {
  return {lhs.red * s, lhs.green * s, lhs.blue * s, lhs.alpha * s};
}
} // namespace Diffuse

#endif // DF_COLOUR_H

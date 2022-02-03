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

#ifndef DF_COLOUR_H
#define DF_COLOUR_H

constexpr float normalize_component(const float comp) {
  return (comp > 1.0f) ? 1.0f : (comp < 0.0f ? 0.0f : comp);
}

namespace Diffuse {
/**
 * @brief Class for representing colours
 */
class Colour {
public:
  /**
   * @brief Initialize a new Colour instance (the colour components are floats
   * in the range [0, 1])
   * @param[in] r the red colour component
   * @param[in] g the green colour component
   * @param[in] b the blue colour component
   * @param[in] a the alpha channel component
   */
  constexpr Colour(float r, float g, float b, float a = 1.0f)
      : alpha{normalize_component(a)}, blue{normalize_component(b)},
        green{normalize_component(g)}, red{normalize_component(r)} {}

  /**
   * @brief Add to another Colour
   * @param rhs The Colour to add the Colour with
   * @return A new Colour instance
   */
  constexpr Colour operator+(const Colour &rhs) const {
    return Colour{red + rhs.red, green + rhs.green, blue + rhs.blue,
                  alpha + rhs.alpha};
  }
  /**
   * @brief Multiply by a scalar
   * @param rhs The scalar to multiply the Colour with
   * @return A new Colour instance
   */
  constexpr Colour operator*(const float rhs) const {
    return Colour{red * rhs, green * rhs, blue * rhs, alpha * rhs};
  }
  constexpr Colour over(const Colour &rhs) const {
    return *this + rhs * (1 - alpha);
  }

private:
  // The individual colour components are floats in the range [0, 1]
  float alpha = 1.0f;
  float blue = 0.0f;
  float green = 0.0f;
  float red = 0.0f;
};
} // namespace Diffuse

#endif // DF_COLOUR_H

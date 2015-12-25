//
// Created by Jim Pelton on 12/24/15.
//

#ifndef BZONE_SLICESET_H
#define BZONE_SLICESET_H

#include <iosfwd>

////////////////////////////////////////////////////////////////////////////////
/// \brief Enumerates the possible sets of slices, each set has quads
///        aligned with the specified plane.
////////////////////////////////////////////////////////////////////////////////
enum class SliceSet : unsigned int {
  XZ,
  YZ,
  XY,
  NoneOfEm,
  AllOfEm
};


////////////////////////////////////////////////////////////////////////////////
/// \brief To string representation of SliceSet.
////////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &ostr, SliceSet s);

#endif //BZONE_SLICESET_H

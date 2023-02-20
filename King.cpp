#include "King.h"
#include <cstdlib>

namespace Chess
{

  // Indicates whether the given start and end coordinates describe a move that
	// would be considered valid for this King if there is no piece at the end
	// position (i.e. this is not a capture move).
  bool King::legal_move_shape(const Position& start, const Position& end) const {
    // return false if piece doesn't move
    if ((start.first == end.first) && (start.second == end.second)) {
        return false;
    }

    // check if movement in any direction is 1
    return (((abs(start.first - end.first) <= 1) && (abs(start.second - end.second) == 1)) || ((abs(start.first - end.first) == 1) && (abs(start.second - end.second) <= 1)));

  }
}
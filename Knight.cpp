#include "Knight.h"
#include <cstdlib>

namespace Chess
{

  // Indicates whether the given start and end coordinates describe a move that
	// would be considered valid for this Knight if there is no piece at the end
	// position (i.e. this is not a capture move).
  bool Knight::legal_move_shape(const Position& start, const Position& end) const {
    
    // return false if piece doesn't move
    if ((start.first == end.first) && (start.second == end.second)) {
        return false;
    }

    // check that if knight moves (2 or 1) move (2 or 1)in other direction
    if (abs(start.first - end.first) == 2) {
      return abs(start.second - end.second) == 1;
    }
    else if (abs(start.first - end.first) == 1) {
      return abs(start.second - end.second) == 2;
    }

    return false;
  }
}
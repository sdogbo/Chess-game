#include "Queen.h"

namespace Chess
{

  // Indicates whether the given start and end coordinates describe a move that
	// would be considered valid for this Queen if there is no piece at the end
	// position (i.e. this is not a capture move).
  bool Queen::legal_move_shape(const Position& start, const Position& end) const {
    //legal if either first or second value stay the same
    if (start.first != end.first) {
      if (start.second == end.second) {
        return true;
      }
    }

    if (start.second != end.second) {
      if (start.first == end.first) {
        return true;
      }
    }

    // return false if piece doesn't move
    if ((start.first == end.first) && (start.second == end.second)) {
        return false;
    }

    // Check for diaganol movement
    return abs(start.first - end.first) == abs(start.second - end.second);

  }
}
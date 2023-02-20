#include "Rook.h"

namespace Chess
{

  // Indicates whether the given start and end coordinates describe a move that
	// would be considered valid for this Rook if there is no piece at the end
	// position (i.e. this is not a capture move).
  bool Rook::legal_move_shape(const Position& start, const Position& end) const {
    //legal if either first or second value stay the same
    if (start.first != end.first) {
      return start.second == end.second;
    }

    if (start.second != end.second) {
      return start.first == end.first;
    }

    // return false if piece doesn't move
    if ((start.first == end.first) && (start.second == end.second)) {
        return false;
    }

    return false;
  }
}
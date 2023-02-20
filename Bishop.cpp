#include "Bishop.h"
#include <cstdlib>

namespace Chess
{
  // Indicates whether the given start and end coordinates describe a move that
	// would be considered valid for this Bishop if there is no piece at the end
	// position (i.e. this is not a capture move).
  bool Bishop::legal_move_shape(const Position& start, const Position& end) const {
    // return false if piece doesn't move
    if ((start.first == end.first) && (start.second == end.second)) {
        return false;
    }

    // Check for diagonal movement
    return abs(start.first - end.first) == abs(start.second - end.second);

  }
}

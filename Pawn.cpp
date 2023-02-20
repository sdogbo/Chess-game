#include "Pawn.h"

namespace Chess
{

  // Indicates whether the given start and end coordinates describe a move that
	// would be considered valid for this Pawn if there is no piece at the end
	// position (i.e. this is not a capture move).
  bool Pawn::legal_move_shape(const Position& start, const Position& end) const {
    // return false if piece doesn't move
    if ((start.first == end.first) && (start.second == end.second)) {
        return false;
    }
    
    //makes sure that the pawn can only move vertically
    if (start.first != end.first) {
      return false;
    } 

    // if piece is white & on home row
    char home_row = is_white() ? '2' : '7';

    //if the piece is on its home row, then it can move 1 or two spaces
    //otherwise it can only move 1 space
    if (start.second == home_row) {
      return is_white() ? (end.second - start.second) <= 2 : (start.second - end.second) <= 2;
    }
    else {
      return is_white() ? (end.second - start.second) == 1 : (start.second - end.second) == 1;
    }
  }

  // Indicates whether the given start and end coordinates describe a move that
	// would be considered valid for this Pawn if there is a piece at the end
	// position (i.e. this is a capture move)
  bool Pawn::legal_capture_shape(const Position& start, const Position& end) const {
    //first part is to make sure the change in x is 1
    //the second part depends on the color and makes sure the change in y is 1
    if (is_white()) {
      return (abs(end.first - start.first) == 1 && end.second - start.second == 1);
    } else {
      return (abs(end.first - start.first) == 1 && start.second - end.second == 1);
    }

  }
}
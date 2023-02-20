#include "Pawn.h"
#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"
#include "Exceptions.h"

namespace Chess
{
  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////

  // Constructor
  Board::Board(){}

  // Destructor
  Board::~Board() {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
        const Piece* piece = (*this)(Position(c, r));
        if (piece) {
          delete piece;
          occ.erase(Position(c, r));
        }
      }
    }
  }

  // Copy constructor
	Board::Board(const Board& org) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
        Position loc(c, r);
        const Piece* piece = org(loc);
        if (piece) {
          this->add_piece(loc, org(loc)->to_ascii());
        }


      }
    }
  }

  // Assignment operator
  Board& Board::operator=(const Board& org) {
    if (this != &org) {
      for(char r = '8'; r >= '1'; r--) {
        for(char c = 'A'; c <= 'H'; c++) {
          Position loc(c, r);
          const Piece* piece = org(loc);
          if (piece) {
            this->add_piece(loc, org(loc)->to_ascii());
          }
        }
      }
    }
    return *this;
  }
  
  // Returns a const pointer to the piece at a prescribed location if it exists,
	// or nullptr if there is nothing there.
  const Piece* Board::operator()(const Position& position) const {
    //look for the position within the map. if the position is unoccupied then it won't be in the map
    //so you return nullptr
    if (occ.find(position) == occ.end()) {
      return nullptr;
    } else {
      //if you get to this block, then the position is occupied and you return the piece.
      Piece* p = occ.at(position);
      return p;
    }
  }
  
  // Attempts to add a new piece with the specified designator, at the given position.
	// Throw exception for the following cases:
	// -- the designator is invalid, throw exception with error message "invalid designator"
	// -- the specified position is not on the board, throw exception with error message "invalid position"
	// -- if the specified position is occupied, throw exception with error message "position is occupied"
  void Board::add_piece(const Position& position, const char& piece_designator) {

    //if the piece_designator is invalid then throw an error
    Piece* p = create_piece(piece_designator);
    if (p == nullptr) {
      throw Exception("invalid designator");
    }

    //if the given position is off the board
    if (position.first < 'A' || position.first > 'H' || position.second < '1' || position.second > '8') {
      throw Exception("invalid position");
    }

    //if the position is already occupied
    if (occ[position] != nullptr) {
      throw Exception("position is occupied");
    }

    occ[position] = p;
  }

  // Attempts to remove a piece at the specified position
  void Board::remove_piece(const Position& position) {
    const Piece* piece = occ[position];
    delete piece;
    occ.erase(position);

  }

  // Displays the board by printing it to stdout
  void Board::display() const {

    //prints the column lettering
    std::cout << "  ";

    //prints the top line of characters
    for (char c = 'A'; c <= 'H'; c++){
      std::cout << c <<  " ";
    }
    std::cout << std::endl;

    for(char r = '8'; r >= '1'; r--){
      //prints the row number
      std::cout << r << " ";

      for(char c = 'A'; c <= 'H'; c++){
		    //Alternate color of each square.
        if((c + r)%2 == 0){
          Terminal::color_all(false, Terminal::WHITE, Terminal:: WHITE);
        } else {
          Terminal::color_all(false, Terminal::CYAN, Terminal::CYAN);
        }

        //check to see if there is a piece at the current position
        //if there is a piece, then output that piece, otherwise, output a few spaces
        Position cur = std::make_pair(c, r);
        if(occ.find(cur) == occ.end()) {
          std::cout << "  ";
        } else {
          std::cout << occ.at(cur)->to_unicode()<< " ";
        }

        
      }
        //you have to reset the terminal color or else that background color will fill the rest of the line
        Terminal::set_default();
        std::cout << " " << r << std::endl;
    }
    
    //prints the column lettering
    std::cout << "  ";
    //prints the bottom line of characters
    for (char c = 'A'; c <= 'H'; c++){
      std::cout << c <<  " ";
    }
    std::cout << std::endl;

  }

  // Returns true if the board has the right number of kings on it
  bool Board::has_valid_kings() const {
    int white_king_count = 0;
    int black_king_count = 0;
    for (std::map<std::pair<char, char>, Piece*>::const_iterator it = occ.begin();
	    it != occ.end();
	    it++) {
      if (it->second) {
	      switch (it->second->to_ascii()) {
	        case 'K':
	          white_king_count++;
	          break;
	        case 'k':
	          black_king_count++;
	          break;
	      }
      }
    }
    return (white_king_count == 1) && (black_king_count == 1);
  }

  /////////////////////////////////////
  // DO NOT MODIFY THIS FUNCTION!!!! //
  /////////////////////////////////////
  // Write the board state to an output stream
  std::ostream& operator<<(std::ostream& os, const Board& board) {
    for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
	const Piece* piece = board(Position(c, r));
	if (piece) {
	  os << piece->to_ascii();
	} else {
	  os << '-';
	}
      }
      os << std::endl;
    }
    return os;
  }

  // Returns true if path of piece is clear
  bool Board::check_path(const Piece* piece, const Position& start, const Position& end) const {
    switch(piece->to_ascii()) {

			case 'K': 
			case 'k': return true;
			case 'Q': 
			case 'q': 
        if ( (start.first != end.first) && (start.second != end.second) ) {
          return check_bishop_path(start, end);
        } else {
          return check_rook_path(start, end);
        }
			case 'B': 
			case 'b': return check_bishop_path(start, end);
      case 'N':
      case 'n': return true;
			case 'R': 
			case 'r': return check_rook_path(start, end);
			case 'P': 
			case 'p': return check_pawn_path(start, end);
			case 'M': 
			case 'm': return check_mystery_path(start, end);
      default: return true;
	
		}
  }

  // Returns true if path of rook is clear
  bool Board::check_rook_path(Position start, Position end) const {
    int i;
    //determines whether you are adding or subtracting to iterate
    if (start.first - end.first + start.second - end.second < 0) {
      i = 1;
    } else {
      i = -1;
    }

    //while the positions are not equal, keep iterating and checking path
    while (! ( (start.first == end.first) &&  (start.second == end.second) ) ) {

      //if you are moving vertically
      if (start.first == end.first) {
        
        start.second += i;
       
        // the last position can be occupied, if capturing
        if (start.second == end.second) {
          break;
        }
        //check to that the current position is unoccupied
        //if it is not, then return false;
        if (occ.find(start) != occ.end()) {
          return false;
        } 
        continue;
      }


      //if you are moving horizontally
      if (start.second == end.second) {

        start.first += i;

        // the last position can be occupied, if capturing
        if (start.first == end.first) {
          break;
        }

        //check to that the current position is unoccupied
        //if it is not, then return false;
        if (occ.find(start) != occ.end()) {
          return false;
        } 
        continue;
      }
    }
    
    // path is clear
    return true;
  }

  // Returns true if path of pawn is clear
  bool Board::check_pawn_path(Position start, Position end) const {
    if (end.second - start.second == 2) {
      Position front(end.first, (end.second - 1));
      if (occ.find(front) != occ.end()) {
        return false;
      }
    }
    else if (end.second - start.second == -2) {
      Position front(end.first, (end.second + 1));
      if (occ.find(front) != occ.end()) {
        return false;
      }
    }
    return true;
  }

  // Returns true if path of bishop is clear
  bool Board::check_bishop_path(Position start, Position end) const {
    int i;
    int j;
    //determines whether you are adding or subtracting to iterate
    if (start.first - end.first < 0) {
      i = 1;
    } else {
      i = -1;
    }

    if (start.second - end.second < 0) {
      j = 1;
    } else {
      j = -1;
    }

    //while the positions are not equal, keep iterating and checking path
    while (! ( (start.first == end.first) &&  (start.second == end.second) ) ) {
      
      // iterate over the path
      start.first += i;
      start.second += j;

      // the last position can be occupied, if capturing
      if ((start.first == end.first) &&  (start.second == end.second)) {
        break;
      }

      // check to see if the position is unoccupied
      if (occ.find(start) != occ.end()) {
        return false;
      }
    }

    return true;
  }

  // Returns true if path of mystery piece is clear
  bool Board::check_mystery_path(Position start, Position end) const {
    if (start.first == end.first || start.second == end.second) {
      return check_rook_path(start, end);
    }
    else if (abs(start.first - end.first) == abs(start.second - end.second)) {
      return check_bishop_path(start, end);
    }
    else {
      return true;
    }
  }

	// returns score of current player
  int Board::get_score(bool white) const {
    int score = 0;

    // for all pieces on the board
		for(char r = '8'; r >= '1'; r--){
			for(char c = 'A'; c <= 'H'; c++){
				Position cur = std::make_pair(c, r);
        
        // if something is at the current position
				if((occ.find(cur) != occ.end())) {
          
          // and it matches the relevant color
          if(occ.at(cur)->is_white() == white) {
            
            // at its point value to the score
            score += occ.at(cur)->point_value();
          }

        // else don't change the score  
        } else { 
          score += 0;
        }
			}

		}
		return score;
  }

  // helper for in_check, gets the king's current coordinates
	Position Board::find_king(bool white) const {
		for(char r = '8'; r >= '1'; r--) {
      for(char c = 'A'; c <= 'H'; c++) {
        Position cur(c, r);

        // if there is something at the current position
        if((occ.find(cur) != occ.end())) {

          // see if it's the king
          const Piece* piece = occ.at(cur);
          if (((piece->to_ascii() == 'K') && white) || ((piece->to_ascii() == 'k') && !white)) {
            return cur;
          }
        }
      }
    }
    // else return dummy posiiton
    return Position('I', '9');
	}


}

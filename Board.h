#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <map>
#include "Piece.h"
#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Mystery.h"


namespace Chess
{
  class Board {

		// Throughout, we will be accessing board positions using Position defined in Piece.h.
		// The assumption is that the first value is the column with values in
		// {'A','B','C','D','E','F','G','H'} (all caps)
		// and the second is the row, with values in {'1','2','3','4','5','6','7','8'}

	public:
		// Default constructor
		Board();

		// Copy constructor
		Board(const Board& org);

		// Destructor
    	~Board();

		// Returns a const pointer to the piece at a prescribed location if it exists,
		// or nullptr if there is nothing there.
		const Piece* operator() (const Position& position) const;

		// Assignment operator
		Board& operator=(const Board& org);

		// Attempts to add a new piece with the specified designator, at the given position.
		// Throw exception for the following cases:
		// -- the designator is invalid, throw exception with error message "invalid designator"
		// -- the specified position is not on the board, throw exception with error message "invalid position"
		// -- if the specified position is occupied, throw exception with error message "position is occupied"
		void add_piece(const Position& position, const char& piece_designator);

		// Attempts to remove a piece at the specified position
		void remove_piece(const Position& position);

		// Displays the board by printing it to stdout
		void display() const;

		// Returns true if the board has the right number of kings on it
		bool has_valid_kings() const;

		// Returns true if path of piece is clear
		bool check_path(const Piece* piece, const Position& start, const Position& end) const;

		// Returns true if path of rook is clear
		bool check_rook_path(Position start, Position end) const;

		// Returns true if path of pawn is clear
		bool check_pawn_path(Position start, Position end) const;

		// Returns true if path of bishop is clear
		bool check_bishop_path(Position start, Position end) const;

		// Returns true if path of mystery piece is clear
		bool check_mystery_path(Position start, Position end) const;

		// returns score of current player
		int get_score(bool white) const;

		// helper for in_check, gets the king's current coordinates
		Position find_king(bool white) const;

		// gets occ
		std::map<Position, Piece*> getOCC() const {return occ;};
    


    

	private:
		// The sparse map storing the pieces, keyed off locations
		std::map<Position, Piece*> occ;

        // Write the board state to an output stream
        friend std::ostream& operator<< (std::ostream& os, const Board& board);

	};
}
#endif // BOARD_H

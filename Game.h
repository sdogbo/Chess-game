#ifndef GAME_H
#define GAME_H

#include <iostream>
#include "Piece.h"
#include "Board.h"
#include "Exceptions.h"

namespace Chess
{

	class Game {

	public:
		// This default constructor initializes a board with the standard
		// piece positions, and sets the state to white's turn
		Game();

		// Copy constructor, which copies the current state of the game
		// Copy constructor, which copies the current state of the game
		Game(bool w, const Board& b) : board(b), is_white_turn(w) {}


		// Returns true if it is white's turn
		/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
		bool turn_white() const { return is_white_turn; }
    
    	/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    	// Displays the game by printing it to stdout
		void display() const { board.display(); }
    
    	/////////////////////////////////////
		// DO NOT MODIFY THIS FUNCTION!!!! //
		/////////////////////////////////////
    	// Checks if the game is valid
		bool is_valid_game() const { return board.has_valid_kings(); }

		// Attempts to make a move. If successful, the move is made and
		// the turn is switched white <-> black. Otherwise, an exception is thrown
		void make_move(const Position& start, const Position& end);
		
		// helper for make_move, checks validity of the move
		void valid_move(const Position& start, const Position& end, bool is_white) const;

		// helper for make_move and find_king
		bool valid_position(const Position& pos) const;

		// helper for make_move, checks validity of the move
		void wont_check(const Position& start, const Position& end, const bool& white) const;

		// helper for make_move, moves the piece
		void move_piece(const Position& start, const Position& end);

		// Returns true if the designated player is in check
		bool in_check(const bool& white) const;

		// Returns true if the designated player is in mate
		bool in_mate(const bool& white) const;

		// Returns true if the designated player is in stalemate
		bool in_stalemate(const bool& white) const;

		// helper for check/checkmate/stalemate. checks if player has moves
		bool can_move(const bool& white) const;

        // Return the total material point value of the designated player
        int point_value(const bool& white) const;

	private:
		// The board
		Board board;

		// Is it white's turn?
		bool is_white_turn;

        // Writes the board out to a stream
        friend std::ostream& operator<< (std::ostream& os, const Game& game);

        // Reads the board in from a stream
        friend std::istream& operator>> (std::istream& is, Game& game);
	};
}
#endif // GAME_H

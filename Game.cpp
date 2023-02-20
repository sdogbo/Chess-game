#include <cassert>
#include "Exceptions.h"
#include "Game.h"

namespace Chess
{
	/////////////////////////////////////
	// DO NOT MODIFY THIS FUNCTION!!!! //
	/////////////////////////////////////
	// This default constructor initializes a board with the standard
	// piece positions, and sets the state to white's turn
	Game::Game() : is_white_turn(true) {
		// Add the pawns
		for (int i = 0; i < 8; i++) {
			board.add_piece(Position('A' + i, '1' + 1), 'P');
			board.add_piece(Position('A' + i, '1' + 6), 'p');
		}

		// Add the rooks
		board.add_piece(Position( 'A'+0 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+7 , '1'+0 ) , 'R' );
		board.add_piece(Position( 'A'+0 , '1'+7 ) , 'r' );
		board.add_piece(Position( 'A'+7 , '1'+7 ) , 'r' );

		// Add the knights
		board.add_piece(Position( 'A'+1 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+6 , '1'+0 ) , 'N' );
		board.add_piece(Position( 'A'+1 , '1'+7 ) , 'n' );
		board.add_piece(Position( 'A'+6 , '1'+7 ) , 'n' );

		// Add the bishops
		board.add_piece(Position( 'A'+2 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+5 , '1'+0 ) , 'B' );
		board.add_piece(Position( 'A'+2 , '1'+7 ) , 'b' );
		board.add_piece(Position( 'A'+5 , '1'+7 ) , 'b' );

		// Add the kings and queens
		board.add_piece(Position( 'A'+3 , '1'+0 ) , 'Q' );
		board.add_piece(Position( 'A'+4 , '1'+0 ) , 'K' );
		board.add_piece(Position( 'A'+3 , '1'+7 ) , 'q' );
		board.add_piece(Position( 'A'+4 , '1'+7 ) , 'k' );
	}
	
	// Attempts to make a move. If successful, the move is made and
	// the turn is switched white <-> black. Otherwise, an exception is thrown
	void Game::make_move(const Position& start, const Position& end) {
	
		// check validity of move
		try { valid_move(start, end, is_white_turn); } catch(const Chess::Exception& e) { throw(e); }

		// check if move exposes check
		try { wont_check(start, end, is_white_turn); } catch(const Chess::Exception& e) { throw(e); }

		// if no exceptions thrown, move piece
		move_piece(start, end);

		// and change turn
		is_white_turn = !is_white_turn;

	}

	// helper for make_move, moves the piece
	void Game::move_piece(const Position& start, const Position& end) {

		const Piece* piece = board(start);
		const Piece* target = board(end);

		// if piece at end, delete it
		if (target != nullptr) {
			board.remove_piece(end); 
		}
		
		// if promoting a pawn, place a queen
		if ((piece->to_ascii() == 'p') && (end.second == '1')) {
			board.add_piece(end, 'q');
		}
		else if ((piece->to_ascii() == 'P') && (end.second == '8')) {
			board.add_piece(end, 'Q');
		}
		
		// else place start piece at end
		else {
			board.add_piece(end, piece->to_ascii());
		}

		// delete piece at start
		board.remove_piece(start);
		

	}

	// helper for make_move and find_king
	bool Game::valid_position(const Position& pos) const {
		return (pos.first >= 'A' || pos.first <= 'H' || pos.second >= '1' || pos.second <= '8');
	}

	// helper for make_move, checks validity of the move
	void Game::valid_move(const Position& start, const Position& end, bool is_white) const {

		// start not on board
		if (!valid_position(start)) {
			throw Exception("start position is not on board");
		}

		// end not on board
		if (!valid_position(end)) {
			throw Exception("end position is not on board");
		}

		// get piece at start and end positions
		const Piece* piece = board(start);
		const Piece* target = board(end);

		// no piece at start
		if (piece == nullptr) {
			throw Exception("no piece at start position");
		}

		// moving wrong color
		if (piece->is_white() != is_white) {
			throw Exception("piece color and turn do not match");
		}

		
		if (target == nullptr) {

			// illegal move shape
			if ( !(piece->legal_move_shape(start, end)) ) {	
				throw Exception("illegal move shape");
			}
			
		} else {

			// capturing own piece
			if (target->is_white() == is_white) {
				throw Exception("cannot capture own piece");
			}

			// illegal capture shape
			if (!(piece->legal_capture_shape(start, end))) {
				throw Exception("illegal capture shape");
			}

		}

		// check if path is clear
		if(!(board.check_path(piece, start, end))) {
			throw Exception("path is not clear");
		}

	}


	// Returns true if the designated player is in check
	bool Game::in_check(const bool& white) const {
		
		// get the king's position
		Position king = board.find_king(white);

		// iterate through the board, checking if there's ever a valid move from the current pos to the king
		for(char r = '8'; r >= '1'; r--) {
      		for(char c = 'A'; c <= 'H'; c++) {
				Position cur(c, r);
				try {
					valid_move(cur, king, !white);
					return true;
				} catch(const Chess::Exception& e) {}
			}
		}

		// only reached if try block always failed, i.e., there were no valid moves, so not in check
		return false;
	}

	// helper for make_move, checks validity of the move
	void Game::wont_check(const Position& start, const Position& end, const bool& white) const {

		// copy the board
		Game rep(white, board);

		// move piece in copy
		rep.move_piece(start, end);

		// check if copied board is in check
		if(rep.in_check(white)) {
			throw Exception("move exposes check");
		}

		return;

	}

	// helper for check/checkmate/stalemate. checks if player has moves (that
	// don't put the player in check)
	bool Game::can_move(const bool& white) const {
		
		std::map<Position, Piece*> o = board.getOCC();
    	
		// for all positions on the board
		for(char r = '8'; r >= '1'; r--) {
      		for(char c = 'A'; c <= 'H'; c++) {
        		Position cur(c, r);
        		
				// if there is something at the current position
        		if((o.find(cur) != o.end())) {
          			const Piece* piece = o.at(cur);

					// if it's the correct color
          			if (piece->is_white() == white) {

						// check if that piece has a valid position to move to
						// that either doesn't cause or blocks a check
            			for(char subr = '8'; subr >= '1'; subr--) {
                			for(char subc = 'A'; subc <= 'H'; subc++) {
								Position end(subc, subr);
								try {
									valid_move(cur, end, white);
									wont_check(cur, end, white);
									return true;
								} catch(const Chess::Exception& e) {}
                			}
						}
					}

        		}
      
      		}
    	}
    	return false;
  	}

	// Returns true if the designated player is in mate
	bool Game::in_mate(const bool& white) const {
		return (in_check(white) && !(can_move(white)));
	}

	// Returns true if the designated player is in stalemate
	bool Game::in_stalemate(const bool& white) const {
		return (!in_check(white) && !(can_move(white)));
	}

    // Return the total material point value of the designated player
    int Game::point_value(const bool& white) const {
		return board.get_score(white);
    }

	// Reads the board in from a stream
    std::istream& operator>> (std::istream& is, Game& game) {
		
		try {
			Board new_board;
			std::string row;
			char cur_row = '8';
			while (is >> row) {

				// if at the last row, set the turn
				if (row.length() == 1) {
					game.is_white_turn = (row[0] == 'w') ? true : false;
					break;
				}

				// else iterate through the current row, adding pieces as you go
				char cur_col = 'A';
				std::stringstream row_ss(row);
				char piece_designator;

				while (row_ss >> piece_designator) {

					// ignore if nothing should be placed
					if(piece_designator != '-') {
						// place piece 
						Position loc(cur_col, cur_row);
						new_board.add_piece(loc, piece_designator);	
					}
					cur_col += 1;

				
					}
					cur_row -= 1;
				}

			// clear board and set game board to new board		
			game.board.~Board(); 
			game.board = new_board;

		} catch(const Chess::Exception& e) { throw(Exception("Cannot load the game!")); }

		return is;
	}

    /////////////////////////////////////
    // DO NOT MODIFY THIS FUNCTION!!!! //
    /////////////////////////////////////
	// Writes the board out to a stream
	std::ostream& operator<< (std::ostream& os, const Game& game) {
		// Write the board out and then either the character 'w' or the character 'b',
		// depending on whose turn it is
		return os << game.board << (game.turn_white() ? 'w' : 'b');
	}
}

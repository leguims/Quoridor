# Quoridor

A Quoridor AI in progress written in C++.

## Todo list

### EPIC "Database":
* [ ] Create class KNOWLEDGE which will be used from IA to play and Game to grow it.
* [ ] Create MAP KNOWLEDGE <board picture(own wall remain, opp wall remain, own position, opp. position, WALL_LIST), MOVE_STATISTICS_LIST >
* [ ] Create SET WALL_LIST <WallPosition>
* [ ] Create MAP MOVE_STATISTICS_LIST <move, statistics(total, win, lose, moves to win/lose average)>
* [ ] Constructor need move list.
* [ ] Method moveList take a "board picture" and return a MOVE_STATISTICS_LIST
* [ ] Method moveStatistics take a "board picture+move" or "MOVE_STATISTICS_LIST+move" and return statistics
* [ ] Method winMove take a MOVE_STATISTICS_LIST and return a move
* [ ] Method loseMove take a MOVE_STATISTICS_LIST and return a move
* [ ] Method drawMove take a MOVE_STATISTICS_LIST and return a move
* [ ] IA uses KNOWLEDGE map to play (define game style from hash table)
* [ ] Fill KNOWLEDGE with saved game.

### EPIC "IA":
* [ ] Define DISCOVER IA : try to play move never played before or the least played. Useless to increase DATABASE.
* [ ] Define WINNER IA : try to play the best move or surrend if no move is winable. Customize with minimum win rate to play.
* [ ] Define ROMANTIC IA : try to play move with the worst draw rate or surrend. Customize with maximum draw rate to play.
* [ ] Define MODEST IA : try to play move with the worst lose rate or surrend. Customize with maximum lose rate to play.
 
### EPIC "OPTIMISE DATABASE":
* scan saved game and show equivalent game files.

## Done list

### EPIC "PDCurses":
* [x] Add PDCurses as submodule
* [x] Make some draft to display board
* [x] Add Display mother class and PDCurses as child
* [x] Use Display from Board class to show evolution of board in real time
* [x] Discard std::cout display with move method.

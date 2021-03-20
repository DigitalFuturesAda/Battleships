# Advanced Programming 2 - Battleships

## Challenge Outline
### Summary & Overview

The challenge of this assignment is to create a playable version of the game battleships in the command line interface 
(CLI). A number of considerations have had to be made for different game modes and various configuration such as a 
dynamic number of ships and a configurable length of each ship - controlled from an ini configuration file.

The default source of the ini configuration file has been noted below and includes documentation stored as comments
(prefixed with `;`). 
```ini
; Defines the dimensions of the board as width * height.
; - Any value greater than or equal to 5 and less than or equal to 80 is valid.
; - Invalid values or repetitions of the Board definition will cause the program to throw and thus halt.
Board = 10x10

; Defines which ships are placed on the board.
; This follows the notation: <ship-name>, <health>, <ship-amount>.
; EG: The input "Carrier, 5, 3" would correlate to 3 carriers on the board all with a health of 5.
;
; - Duplicate ship-name values will result in an error and the program halting. If a ship is set to
;   a health of n, it can not be redefined later on, nor can the ship-amount value be redefined later on.
; - Setting the value to 0 is valid and will cause that specific ship not to be placed on the board,
;   however values less than 0 will also cause the program to halt.
; - Setting the ship health to a value greater than the width or height of the board will cause the program
;   to halt.
;
; - The ship-name must conform (case sensitive) to the following values: Carrier, Battleship, Destroyer,
;                                                                        Submarine, Patrol Boat
Ship = Carrier, 5, 1
Ship = Battleship, 4, 1
Ship = Destroyer, 3, 1
Ship = Submarine, 3, 1
Ship = Patrol Boat, 2, 1
```

The board has a minimum width and height of 5 and a max of 80. The ships are represented as `{ship-name}`, `{ship-health}`,
`{number-of-ships}`. For logical reasons the boat health which represents the length can not exceed the width or height
or the board, nor can it be less than 1. If any of the configuration options are invalid the program will alert the user
of this - providing the source of the offending line - and then exit the program execution.

In a standard game of battleships, there are two players who will play competitively against each other following a turn
based approach. Each player can see their own game board and a hit board, however they are not allowed to view the opposing
players boards, to prevent this happening I've implemented control sequences to clear the console and then displayed outputs
prompting the user to pass the device on to the next user.

One of the requirements were to include a number of different game modes, these included options for a computer versus 
computer simulation, a salvo game mode which means you can shoot as many missiles as you have boats remaining on the 
board and a version with mines which are randomly deployed and if hit will destroy the 8 adjacent nodes. Furthermore 
an advanced warhead deployment algorithm has been developed for the player vs computer and computer vs computer
simulation game modes which instead of being random would use a number of heuristics to choose where to fire the next
warheads (rather than a random algorithm).

### Proposed Solution
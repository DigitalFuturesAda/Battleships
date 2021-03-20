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
#### Proposed components
- `GameGrid Class` 
    - **Displays the location and status of the players ships and - if applicable - the location and status of mines**
    - This will be implemented as a matrix using the built in `vector` class and will be set to the default `GridNodes`
      value, that being `UNKNOWN` 
    - Handles formatting the different `GridNodes` using a `formatNode(GridNodes node)` method
    - Validates and displays attempts to place `GridNodes` and series of `GridNodes` onto the `GameGrid`
    - Exposes a public API to:
        - Receive warhead strikes at any valid position on the board from the opposing player
        - Render the board (`renderBoard`), this will iterate through each row of the matrix, format the node as per the
          `formatNode` specification and writes this to a multiline `std::stringstream` for use by the method caller.
    - `HitGrid Class · Inherits from GameGrid`
        - **Displays where the player has attempted to hit and whether that hit was valid or invalid**
        - Exposes two public APIs to mark successful and unsuccessful warhead strikes
    
- `Player Class`
    - **Handles player interaction with the two boards and how the player interacts with the opposing player**
    - Handles storing and setting a reference to the opposing player
    - Stores a `vector` of the players `Ships` and their status
    - Expose public APIs to
        - Automatically deploy mines onto the `GameBoard`
        - Automatically and manually position ships on the `GameBoard`
        - Render the players `GameBoard` and `HitGrid` by delegating to their relevant `#renderBoard` methods
        - Render an interface to let the player deploy any ship that is stationary
        - Get the status of the players `Ships`
        - Deploy warheads at the opposing player after passing validation checks
        - Handle mine detonation logic w/ recursive functionality
        - A boolean which correlates to whether all their ships have sunk
    
- `Ship Class`
    - **Stores information pertaining to the status of the ships on the players `PlayerGrid`**
    - Stores the following properties:
        - Health
        - Max Lives
        - Status `(SUNK, STATIONARY, DEPLOYED)`
        - Orientation `(VERTICAL, HORIZONTAL)`
        - ID - random ID between `INT_MIN` and `INT_MAX` as a differentiator if there are multiple on the board
        - `Vector` of coordinates where the ship is placed if not `STATIONARY`
    - Exposes public setters and getters for all these properties
    - Exposes public APIs to:
        - Check if a given coordinate intersects with any value in the vector of stored coordinates
        - Get the name of the ship (eg: `Carrier` or `Battle Ship`)
    
- `ConfigFileParser Class`
    - **Used to parse the contents of a given configuration file**
    - Parses the contents of a given configuration file into a determinate structure, accessible via a number of 
      public APIs. The INI configuration specified in the brief allows duplicate keys which meant special consideration 
      has been implemented for this.
    - If validation fails for any reason, the reason will be output to the user and the program will exit.
    - Given the format:
      ```ini
      Boat: Carrier, 5
      Boat: Battleship 4
      Boat: Submarine 3
      ```
    - The INI parser will push these into a `map` such that the key `Boat` points to a `vector` containing `Carrier, 5`,
      `Battleship 4` and `Submarine 3`.
    - Two public APIs will be exposed to extract these values:
        - `getProperties(std::string property)` - fetch multiple values as a `vector` from the `map` given the property
        - `getProperty(std::string property)` - fetch a single property from the `vector`, this method will throw if the
          vector contains multiple values, it exists solely as a helper method for fetching configuration options that
          should be set at most once in the INI file
          
- `ConfigValidator Class`
    - **Used to validate the properties provided by the `ConfigFileParser`**
    - Validates the properties and then stores them in a cache to prevent unnecessary computation. If validation fails
      (EG the health of a ship exceeds the height or width of the board) then an error message will be logged and the 
      program will exit.
    - Exposes public APIs to:
        - Get the board dimensions
        - Get a `vector` of different ships to be placed on the board
        - Search through a `map` of ships to get the health of a given valid ship
    
- `ConfigSingleton Class`
    - **Provides a single entry point to the `ConfigValidator` which can be used from any class at anypoint in the game**
    
- `Menu Class`
    - **Renders a menu to the user and validates their input as a value between 1 and n; correlating to a menu item**
    - Stores a `vector` of `pairs<std::string, MenuGameConfiguration>` which correlate to:
        - `<std::string>` - The name of the option, eg: `Player vs Computer w/ Salvo`
        - `<MenuGameConfiguration>` - A struct containing the relevant configuration, EG:
            - PlayerOneType: `PLAYER`
            - PlayerTwoType: `COMPUTER`
            - SalvoGameMode: `true`
            - HiddenMines:   `false`
    - Exposes two public APIs:
        - `renderMenu` - renders a menu to the user
        - `getConfigurationOption` - returns which menu item the user selected
    
- `HostController class`
    - **Checks whether a player has won and provides logic to switch between the currently active player**
    - Provides public APIs to check if either player has won, and if so clear the console and print the winning screen
    - Provides an API to switch the currently active player
    
- `Main`
    - Initiate the various classes
    - Display the menu and sync these options with the initiated classes
    - Start the game thread and use the `HostController` to switch between the active players and render the win screen
      once either player has won/lost.
      
#### UML diagram
TODO...
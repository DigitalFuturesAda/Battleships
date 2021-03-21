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

Whilst playing against the computer both the players hit board and game board should be shown as well as the computers
boards to aid in testing. Below is a screenshot of what this looks like:

![Player vs Computer](readme-assets/images/player-vs-computer-starter.png)

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
![UML diagram](readme-assets/images/UML.png)

### Initial working plan
#### Overall approach

The overall approach to this coursework was to test early and push the limits of my code as far as possible, by doing
this I would discovered any faults early on and thus, be able to fix them whilst the code was still fresh in my mind.
I followed a sprint like pattern to software development, where I quickly iterated on different ideas to test them for
robustness and feasibility, for instance I wasn't sure what the technical constraints for displaying the grid would be,
so I was crossed between representing the grid as a matrix (an array of arrays) or instead a more abstract data structure.
I tested both of these different approaches and settled with using a matrix. Once I had prototyped what I believed to be
a feasible starter product, I then followed the waterfall methodology. This meant that I wrote and took a hermetic approach
to testing. 

I also wanted to make sure I made good use of Object Orientated Programming in this assignment, coming from a Java background,
thinking of different components as objects came naturally. My approach for this was to have as little code as possible
in the `main.cpp` file, ideally different objects would be instantiated in the `main` and various public APIs would be
called which would independently mutate various properties in the object and it's child objects. In C++ you can specify
public and private methods and properties, my approach for naming was to keep my code as self documenting as possible.
Any API you call would take logical parameters and give a response which could be inspected for correctness. I planned
on making heavy use of structures, which provide great type completion across different classes.

#### Development Strategy
I created a components folder which contains various folders containing the different classes. To make the separation clear
each class had a header and a cpp file, any structs would be defined in the header and code would never be placed in the
header file as per Google's style guide (https://google.github.io/styleguide/cppguide.html) which I followed for the
duration of this coursework. Each components will be tested manually before being commited to Github to ensure their
quality.

For instance the `Player` class is positioned in: `components/player/...` which contains the `Player` class and a number
of utilities and classes which complement the functionality of the `Player` class. I decided not to create a unique folder
for each class as that would clutter the codebase too much. Instead the folder names have been used to let the programmer
quickly identify the main use of the code positioned within the folder:

- `components/config:` Handles parsing, validating and providing a singleton entry point.
    - `ConfigFileParser, ConfigSingleton, ConfigSingleton`
- `components/grid:` Handles the hit grid and main game grid.
    - `GameGrid, HitGrid`
- `components/menu:` Displays the grid to the user and validates their inputs
    - `MenuHelper`
- `components/player:` Handles all player interaction, detects if either player has won, holds a reference to each player
                       and provides a two way reactive data binding between the player class, the inputs and the main
                       program file.
    - `Player, HostController, GameFlowController`
- `components/ship:` Stores the ship class and information pertaining to the ship
    - `Ship`
- `components/util:` Various utility methods such as IO functions, random methods and string manipulation.
    - `io, rand, strings`

#### Approach to quality
Due to time and technical constraints, I decided not to write any tests, however as previously noted I thoroughly tested
all code. Whilst implementing the different objects, I followed the programming technique of defensive programming
(https://en.wikipedia.org/wiki/Defensive_programming). Defensive programming is a form of defensive design, in which 
the program should be able to mitigate unforeseen circumstances -  each component is designed with the assumption people 
will try and break it, meaning every single edge case was accounted for and code put in place to mitigate erroneous 
inputs. I also followed a structured testing schedule, testing my inputs and functions with erroneous, boundary extreme  
and normal data to make sure erroneous and extreme data did not cause my program to stop functioning or break in
unintended ways (logic errors).

### Decomposition into Epics
To help me plan and manage the implementation of the game, I've broken the various components down into epics:
    - Epic 1: Main file
    - Epic 2: Board
    - Epic 3: Ship
    - Epic 4: Player
    - Epic 5: Parsing

None of these epics strictly required the other to be created, I decided to handle the parsing epic last because that was
a rather self contained project. Due to the object orientated approach, configuration could be passed into any of the
other objects from the `ConfigSingleton` during the instantiation phase of each object.

### Object-Oriented Design Ideas and Phased Breakdown
All but epic 1 will be implemented as classes with each epic being developed in a non linear order; rather ad-hoc when
needed. Generally each epic asides from the 5th will be developed in parallel. As I broke these problems down into
sub-problems, I realised there was a great need for interoperability between the different classes, for instance the Player
class would hold each Board and the Board class would require knowledge of which ships the player had, but the Player
would directly own the a vector of the players ships, rather than the board owning the ships. 

The first epic also included a number of miscellaneous tasks, such as writing functions to generate random numbers, get
input from the user and validate them in self contained methods and a number of time saving utility-focused string and
number methods.
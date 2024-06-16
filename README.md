# TETRIS GAME

Implementation of Tetris game with extended features like piece shadowing and hard drop.

## Extended features

- **Single player / two players game**. Game can be built as single player or two players game by changing number of parameters during Game instantiation.

Example of single player
```cpp
Game<1> game{window_height, window_width, game::font_type, player_1};
```
Example of two players game
```cpp
Game<2> game{window_height, window_width, game::font_type, player_1, player_2};
```
- **Saving game**. Game can be paused and saved during gameplay.
- **Loading saved game**. Game can be loaded from saved file during the start screen.
- **Pre-computed tetrinos**. Tetrinos and their rotations are pre-computed at game start. Creating/Rotating tetrino is just returning pointer from circural list.

## Dependencies

List of not included libraries

- **raylib** for rendering. For installing see raylib's [documentation](https://github.com/raysan5/raylib?tab=readme-ov-file)

List of libraries included in source files

- **nlohmann::json** for saving and loading game
- **tinyfiledialogs** for cross-platform work with file explorer

## Build

Game can be built in multiple platforms(Linux, macOS, Windows). For building is used CMake.

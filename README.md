# Snake
A generic Snake game in form of an Arduino library. Intialize with `Snake::Snake(unsigned int width, unsigned int height)`and start with `void Snake::start_game()`. Step through the game with `bool Snake::step(DirectionType_t dir)` and draw it however you like with `bool Snake::pixel_is_set(unsigned int x, unsigned int y)`. The stepper function will return `false` when the current game ended.

# Usage
1. move this folder in /Arduino/libraries
2. run examples/Snakegame/Snakegame.ino to test it

# Example

[![Flipdot running Snake](https://img.youtube.com/vi/TgFTgHuQ3Pw/0.jpg)](https://www.youtube.com/watch?v=TgFTgHuQ3Pw)

Find the code [here!](https://github.com/nerdmanufaktur/flipdot/blob/master/Arduino/examples/Wemos_Demo/Wemos_Demo.ino)

// - - - - -
// Snake - An Arduino library for running a snake game
// Snake.cpp: Library implementation file
//
// Copyright (C) 2016 kryptokommunist <marcus.ding<at>student.hpi.de>
// This work is licensed under a GNU style license.
//
// Last change: kryptokommunist
//
// Documentation and samples are available at https://github.com/kryptokommunist/Snake
// - - - - -

/* ----- LIBRARIES ----- */
#include "Snake.h"

/*
Initialize the Snake game
*/
Snake::Snake(unsigned int width, unsigned int height) {
    DBG_OUTPUT_PORT_NL("Creating Snake object, setting with & height")
    screen_width = width;
    screen_height = height;
    screen_buffer = new bool*[screen_width];
    //intialize screen buffer
    for(int i = 0; i < screen_width; i++){
      screen_buffer[i] = new bool[screen_height];
    }
    randomSeed(analogRead(A0));
}

/*
start a new snake game
*/
void Snake::start_game() {
    for(int i = 0; i < screen_width; i++){
      memset(screen_buffer[i], false, screen_height*sizeof(bool));
    }
    snake.clear();
    snake_stomach.clear();
    score = 0;
    DBG_OUTPUT_PORT_NL("Starting Snake game")
    place_food();
    unsigned int head_x = screen_width/2;
    unsigned int head_y = screen_height/2;
    unsigned int tail_x = head_x - SNAKE_STARTING_LENGTH+1;
    unsigned int tail_y = head_y;
    Interval head = std::make_tuple(head_x, head_y, RIGHT);
    Interval end_point = std::make_tuple(tail_x, tail_y, END);
    snake.push_front(end_point);
    snake.push_front(head);
    for(int i = 0; i < SNAKE_STARTING_LENGTH; i++){
        buffer_draw_point(true, tail_x+i, tail_y);
    }
}

/*
draw point in buffer
*/
void Snake::buffer_draw_point(bool val, unsigned int x, unsigned int y) {
    DBG_OUTPUT_PORT_NL("drawing point in buffer")
    screen_buffer[x][y] = val;
}

/*
get the snakes head for gods sake
*/
Snake::Interval Snake::snake_head() {
    DBG_OUTPUT_PORT_NL("Getting current head");
    return snake.front();
}

/*
get current direction of the head of the snake
*/
Snake::Direction Snake::snake_heading() {
    DBG_OUTPUT_PORT_NL("Getting current direction");
    Interval snake_head = snake.front();
    return std::get<2>(snake_head);
}

/*
get tail of the snake (not endpoint)
*/
Snake::Interval Snake::snake_tail() {
    DBG_OUTPUT_PORT_NL("Getting tail");
    Interval snake_end_point = snake.back();
    /*
      hacky solution that should be improved
    */
    snake.pop_back();
    Interval snake_tail = snake.back();
    snake.push_back(snake_end_point);
    return snake_tail;
}

/*
get end_point of the snake (not tail)
*/
Snake::Interval Snake::snake_end_point() {
    DBG_OUTPUT_PORT_NL("Getting end point");
    return snake.back();
}

/*
end the snake game
*/
void Snake::end_game() {
    DBG_OUTPUT_PORT_NL("Game over!");
    //render some nice stats or stats
}

/*
move snake a step into given direction. returns true if the snake digested food (tail gets longer).
*/
bool Snake::snake_slither(Direction dir) {
    //handle head
    DBG_OUTPUT_PORT_NL("Snake slithers")
    if(dir != snake_heading()){
      Interval new_head = snake_head();
      std::get<2>(new_head) = dir;
      snake.push_front(new_head);
    }

    Interval head = snake_head();
    Direction heading = snake_heading();
    if(heading == UP){
      std::get<1>(head) += 1;
    } else if (heading == DOWN) {
      std::get<1>(head) -= 1;
    } else if (heading == LEFT) {
      std::get<0>(head) -= 1;
    } else if (heading == RIGHT){
      std::get<0>(head) += 1;
    } else {
      DBG_OUTPUT_PORT_NL("Snake slither ERROR!")
    }
    snake.pop_front();
    snake.push_front(head);

    //handle tail
    Interval end_point = snake_end_point();
    Interval tail = snake_tail();
    unsigned int tail_x = std::get<0>(tail);
    unsigned int tail_y = std::get<1>(tail);
    unsigned int tail_direction = std::get<2>(tail);

    if(!snake_stomach.empty()){
      Interval food = snake_stomach.back();
      unsigned int food_x = std::get<0>(food);
      unsigned int food_y = std::get<1>(food);
      if(std::get<0>(end_point) == food_x && std::get<1>(end_point) == food_y) {
        snake_stomach.pop_back();
        return true;; //don't shorten the tail this round
      }
    }

    if(tail_direction == UP){
      std::get<1>(end_point) += 1;
    } else if (tail_direction == DOWN) {
      std::get<1>(end_point) -= 1;
    } else if (tail_direction == LEFT) {
      std::get<0>(end_point) -= 1;
    } else if (tail_direction == RIGHT){
      std::get<0>(end_point) += 1;
    }
    unsigned int end_point_x = std::get<0>(end_point);
    unsigned int end_point_y = std::get<1>(end_point);
    snake.pop_back();
    snake.push_back(end_point);
    if(tail_x == end_point_x && tail_y == end_point_y){
      snake.pop_back(); //remove end point
      snake.pop_back(); //remove former tail
      snake.push_back(end_point); //add end_point again
    }
    return false;
}

/*
stepping to next game state, returns false if game over
*/
bool Snake::step(Direction dir) {
    DBG_OUTPUT_PORT_NL("Snake game stepping")
    Interval previous_end_point = snake_end_point();
    if(dir == AUTO) {
      dir = snake_autopilot();
    }
    dir = handle_opposite_direction(dir);
    bool had_food = snake_slither(dir);
    DBG_OUTPUT_PORT_NL("Snake finished slithering")
    Interval updated_head = snake_head();
    unsigned int updated_head_x = std::get<0>(updated_head);
    unsigned int updated_head_y = std::get<1>(updated_head);
    if(updated_head_x == food_x && updated_head_y == food_y){ //snake found food
      snake_stomach.push_front(std::make_tuple(food_x,food_y,FOOD));
      score++;
      place_food();
    } else { //no food so we check if the snake collided with itself
      if(snake_collision(updated_head_x,updated_head_y)){
        end_game();
        return false;
      }
    }
    if(snake_head_is_in_bounds()){ //has the snake collided with the screen borders?
      DBG_OUTPUT_PORT_NL("Snake head is in bounds")
      buffer_draw_point(true,updated_head_x,updated_head_y);
      buffer_draw_point(had_food,std::get<0>(previous_end_point),std::get<1>(previous_end_point));
    } else { //the needs to switch sides through a wormhole
      /*
        TO DO
      */
      DBG_OUTPUT_PORT_NL("Snake head is NOT in bounds")
      return false;
    }
    return true;
}

/*
checks for collision with snake, returns true/false
*/
bool Snake::snake_collision(unsigned int x, unsigned int y) {
    if(snake_head_is_in_bounds() && screen_buffer[x][y]){
      return true;
    }
    return false;
}

/*
places food for the snake
*/
void Snake::place_food() {
    DBG_OUTPUT_PORT("Placing food");
    unsigned int x;
    unsigned int y;
    do {
        x = (unsigned int) random(screen_width);
        y = (unsigned int) random(screen_height);
    } while(pixel_is_set(x,y));
    buffer_draw_point(true,x,y);
    food_x = x;
    food_y = y;
}

/*
returns true if pixel at given position is set
*/
bool Snake::pixel_is_set(unsigned int x, unsigned int y) {
      return screen_buffer[x][y];
}

/*
returns true if the current snake is within the screen bounds
*/
bool Snake::snake_head_is_in_bounds() {
    Interval head = snake_head();
    unsigned int head_x = std::get<0>(head);
    unsigned int head_y = std::get<1>(head);
    return (head_x >=0 && head_x < screen_width) && (head_y >= 0 && head_y < screen_height);
}

/*
return old heading if new direction is opposite, else just return given direction
*/
Snake::Direction Snake::handle_opposite_direction(Direction dir) {
  Direction current_head_dir = snake_heading();
  if(is_opposite[current_head_dir][dir]) {
    return current_head_dir;
  }
  return dir;
}

/*
heuristic to keep the snake going by itself
TO DO: Implement a more intelligent algorithm
*/
Snake::Direction Snake::snake_autopilot() {
  Interval head = snake_head();
  uint head_x = std::get<0>(head);
  uint head_y = std::get<1>(head);
  Direction heading = std::get<2>(head);

  int delta_x = head_x - food_x;
  int delta_y = head_y - food_y;

  if(delta_x != 0){
    if(delta_x < 0) {
      if(heading != LEFT) {
        return RIGHT;
      }
      return DOWN;
    } else {
      if(heading != RIGHT) {
        return LEFT;
      }
      return DOWN;
    }
  }

  if(delta_y != 0){
    if(delta_y < 0) {
      if(heading != DOWN) {
        return UP;
      }
      return RIGHT;
    } else {
      if(heading != UP) {
        return DOWN;
      }
      return RIGHT;
    }
  }

}


// ---- Debug mode functions ----
#if defined(DBG)

  void Snake::render_frame_buffer() {
      for(unsigned int y = 0; y < screen_height; y++){
        DBG_OUTPUT_PORT("|");
        for(unsigned int x = 0; x < screen_width; x++){
          if(pixel_is_set(x,y)){
              DBG_OUTPUT_PORT("x");
          } else {
              DBG_OUTPUT_PORT(" ");
          }
        }
        DBG_OUTPUT_PORT_NL("|");
      }
      for (IntervalList::const_iterator iterator = snake.begin(); iterator != snake.end(); ++iterator) {
          DBG_OUTPUT_PORT(std::get<0>(*iterator))
          DBG_OUTPUT_PORT(",");
          DBG_OUTPUT_PORT_NL(std::get<1>(*iterator))
      }
      DBG_OUTPUT_PORT_NL(snake_heading())
  }

#endif

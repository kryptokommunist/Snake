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
}

/*
start a new snake game
*/
void Snake::start_game() {
    for(int i = 0; i < screen_width; i++){
      memset(screen_buffer[i], false, screen_height*sizeof(bool));
    }
    snake.clear();
    DBG_OUTPUT_PORT_NL("Starting Snake game")
    unsigned int head_x = screen_width/2;
    unsigned int head_y = screen_height/2;
    unsigned int tail_x = head_x - SNAKE_STARTING_LENGTH+1;
    unsigned int tail_y = head_y;
    IntervalType_t head = std::make_tuple(head_x, head_y, SNAKE_RIGHT);
    IntervalType_t end_point = std::make_tuple(tail_x, tail_y, SNAKE_END);
    snake_push_segment(end_point);
    snake_push_segment(head);
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
add a new interval to the snake
*/
void Snake::snake_push_segment(IntervalType_t interval) {
    DBG_OUTPUT_PORT_NL("Pushing new interval to snake");
    snake.push_front(interval);
}

/*
get current direction of the head of the snake
*/
IntervalType_t Snake::snake_head() {
    DBG_OUTPUT_PORT_NL("Getting current head");
    return snake.front();
}

/*
get the snakes head for gods sake
*/
DirectionType_t Snake::snake_heading() {
    DBG_OUTPUT_PORT_NL("Getting current direction");
    IntervalType_t snake_head = snake.front();
    return std::get<2>(snake_head);
}

/*
get tail of the snake (not endpoint)
*/
IntervalType_t Snake::snake_tail() {
    DBG_OUTPUT_PORT_NL("Getting tail");
    IntervalType_t snake_end_point = snake.back();
    /*
      hacky solution that should be improved
    */
    snake.pop_back();
    IntervalType_t snake_tail = snake.back();
    snake.push_back(snake_end_point);
    return snake_tail;
}

/*
get end_point of the snake (not tail)
*/
IntervalType_t Snake::snake_end_point() {
    DBG_OUTPUT_PORT_NL("Getting end point");
    return snake.back();
}

/*
end the snake game
*/
void Snake::end_game() {
    DBG_OUTPUT_PORT_NL("Game over!");
//render some nice stats or sth
}

/*
move snake a step into given direction
*/
void Snake::snake_slither(DirectionType_t dir) {
    //handle head
    DBG_OUTPUT_PORT_NL("Snake slithers")
    if(dir != snake_heading()){
      IntervalType_t new_head = snake_head();
      std::get<2>(new_head) = dir;
      snake_push_segment(new_head);
    }

    IntervalType_t head = snake_head();
    DirectionType_t heading = snake_heading();
    if(heading == SNAKE_UP){
      std::get<1>(head) += 1;
    } else if (heading == SNAKE_DOWN) {
      std::get<1>(head) -= 1;
    } else if (heading == SNAKE_LEFT) {
      std::get<0>(head) -= 1;
    } else if (heading == SNAKE_RIGHT){
      std::get<0>(head) += 1;
    } else {
      DBG_OUTPUT_PORT_NL("Snake slither ERROR!")
      DBG_OUTPUT_PORT_NL("Snake slither ERROR!")
      DBG_OUTPUT_PORT_NL("Snake slither ERROR!")
    }
    snake.pop_front();
    snake_push_segment(head);

    //handle tail
    IntervalType_t end_point = snake_end_point();
    IntervalType_t tail = snake_tail();
    unsigned int tail_x = std::get<0>(tail);
    unsigned int tail_y = std::get<1>(tail);
    unsigned int tail_direction = std::get<2>(tail);
    if(tail_direction == SNAKE_UP){
      std::get<1>(end_point) += 1;
    } else if (tail_direction == SNAKE_DOWN) {
      std::get<1>(end_point) -= 1;
    } else if (tail_direction == SNAKE_LEFT) {
      std::get<0>(end_point) -= 1;
    } else if (tail_direction == SNAKE_RIGHT){
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
}

/*
stepping to next game state, returns false if game over
*/
bool Snake::step(DirectionType_t dir) {
    DBG_OUTPUT_PORT_NL("Snake game stepping")
    IntervalType_t previous_end_point = snake_end_point();
    snake_slither(dir);
    IntervalType_t updated_head = snake_head();
    unsigned int updated_head_x = std::get<0>(updated_head);
    unsigned int updated_head_y = std::get<1>(updated_head);
    if(snake_collision(updated_head_x,updated_head_y)){
      end_game();
      return false;
    } else if(snake_head_is_in_bounds()){
      /*
        TO DO
      */
      buffer_draw_point(true,updated_head_x,updated_head_y);
      buffer_draw_point(false,std::get<0>(previous_end_point),std::get<1>(previous_end_point));
    } else {
      /*
        TO DO
      */
      DBG_OUTPUT_PORT_NL("Snake step ERROR!")
      DBG_OUTPUT_PORT_NL("Snake step ERROR!")
      DBG_OUTPUT_PORT_NL("Snake step ERROR!")
    }
    return true;
}

/*
checks for collision with snake, returns true/false
*/
bool Snake::snake_collision(unsigned int x, unsigned int y) {
    if(screen_buffer[x][y]){
      /*
        to do: implement checking for food
      */
      return true;
    }
    return false;
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
    IntervalType_t head = snake_head();
    unsigned int head_x = std::get<0>(head);
    unsigned int head_y = std::get<1>(head);
    return (head_x >=0 && head_x < screen_width) && (head_y >= 0 && head_y < screen_height);
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
      for (IntervalListType_t::const_iterator iterator = snake.begin(); iterator != snake.end(); ++iterator) {
          DBG_OUTPUT_PORT(std::get<0>(*iterator))
          DBG_OUTPUT_PORT(",");
          DBG_OUTPUT_PORT_NL(std::get<1>(*iterator))
      }
      DBG_OUTPUT_PORT_NL(snake_heading())
  }

#endif

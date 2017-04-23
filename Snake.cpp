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
Initialize the Snake
*/
Snake::Snake(unsigned int width, unsigned int height) {
    DBG_OUTPUT_PORT_NL("Creating Snake object, setting with & height")
    screen_width = width;
    screen_height = height;
}

/*
start a new snake game
*/
void Snake::start_game() {
    DBG_OUTPUT_PORT_NL("Starting Snake game")
    head_pos.x = screen_width/2;
    head_pos.y = screen_height/2;
    tail_pos.x = head_pos.x - SNAKE_STARTING_LENGTH;
    tail_pos.y = head_pos.y;
    head_heading = SNAKE_RIGHT;
    tail_heading = SNAKE_RIGHT;
    IntervalType_t interval = std::make_tuple(head_pos.y, tail_pos.x, head_pos.y, SNAKE_RIGHT);
    x_intervals.push_front(interval);
    snake.push_front(interval);
}

/*
end the snake game
*/
void Snake::end_game() {
    DBG_OUTPUT_PORT_NL("Game over!");
//render some nice stats or sth
}

/*
stepping to next game state, returns false if game over
*/
bool Snake::step(DirectionType_t dir) {
    DBG_OUTPUT_PORT_NL("Snake game stepping")
    DBG_OUTPUT_PORT("x_intervals: ")
    DBG_OUTPUT_PORT_NL(x_intervals.size())
    DBG_OUTPUT_PORT("y_intervals: ")
    DBG_OUTPUT_PORT_NL(y_intervals.size())
    DBG_OUTPUT_PORT("snake: ")
    DBG_OUTPUT_PORT_NL(snake.size())
    if(snake_will_be_in_bounds(dir)){
        previous_head_pos = head_pos;
        if(dir == SNAKE_UP){
            head_pos.y += 1;
        } else if(dir == SNAKE_DOWN) {
            head_pos.y -= 1;
        } else if(dir == SNAKE_LEFT){
            head_pos.x -= 1;
        } else if(dir == SNAKE_RIGHT) {
            head_pos.x += 1;
        }
        handle_intervals(dir);
    } else {
        //game over
        end_game();
        return false;
    }
    if(collision_detected()){
        //game over again
        end_game();
        return false;
    }
    return true;
}

/*
returns true if pixel at given position is set
*/
bool Snake::pixel_is_set(unsigned int x, unsigned int y) {
//stuff missing like food :))

//
      if(snake_is_at_position(x, y)){
          return true;
      }
      return false;
}

/*
returns true if the snakes body is at given position
*/
bool Snake::snake_is_at_position(unsigned int x, unsigned int y) {
    for(auto iterator = x_intervals.begin(); iterator != x_intervals.end(); ++iterator) {
        IntervalType_t x_interval = *iterator; // As above
        if(point_is_in_interval(x_interval, y, x)) {
            return true;
        }
    }
    for(auto iterator = y_intervals.begin(); iterator != y_intervals.end(); ++iterator) {
        IntervalType_t y_interval = *iterator; // As above
        if(point_is_in_interval(y_interval, x, y)) {
            return true;
        }
    }
    return false;
}


/*
returns true if pint_interval_dimension lies within interval and the point_fixed_dimension is equal to the fixed dimension of the interval
*/
bool Snake::point_is_in_interval(IntervalType_t interval, unsigned int point_fixed_dimension, unsigned int point_interval_dimension) {
    bool test = std::get<0>(interval) == point_fixed_dimension;
    test &= std::get<1>(interval) <= point_interval_dimension;
    test &= std::get<2>(interval) >= point_interval_dimension;
    return test;
}

/*
returns true if the next step will be within the screen bounds
*/
bool Snake::snake_will_be_in_bounds(DirectionType_t dir) {
    if(dir == SNAKE_UP){
        return head_pos.y > 0;
    } else if(dir == SNAKE_DOWN) {
        return head_pos.y > screen_height-1;
    } else if(dir == SNAKE_LEFT){
        return head_pos.x > 0;
    } else if(dir == SNAKE_RIGHT) {
        return head_pos.x < screen_width-1;
    }
}

/*
returns true if the snake collided with itself
*/
bool Snake::collision_detected() {
    DBG_OUTPUT_PORT_NL("Collision detection")
    return snake_is_at_position(head_pos.x, head_pos.y);
}

/*
returns true if the direction is x
*/
bool Snake::is_horizontal_direction(DirectionType_t dir) {
    return dir == SNAKE_LEFT || dir == SNAKE_RIGHT;
}

/*
inserts new interval if necessary, important!: call after updating head_pos and previous_head_pos
*/
void Snake::handle_intervals(DirectionType_t dir) {
    DBG_OUTPUT_PORT_NL("Handling intervals")
    if(dir != head_heading) {
        if(is_horizontal_direction(dir)) {
            IntervalType_t interval = create_new_x_interval(dir);
            x_intervals.push_front(interval);
            snake.push_front(x_intervals.front());
        } else {
            IntervalType_t interval = create_new_y_interval(dir);
            y_intervals.push_front(interval);
            snake.push_front(y_intervals.front());
        }
        head_heading = dir;
    } else {
        update_head_interval();
    }
    update_tail_interval();
}

/*
updates the front head interval, should be called when the head_heading is unchanged
*/
void Snake::update_head_interval() {
    DBG_OUTPUT_PORT_NL("Updating head interval")
    IntervalType_t snake_head = snake.front();
    DirectionType_t dir = std::get<3>(snake_head);
    unsigned int start = std::get<1>(snake_head);
    unsigned int end = std::get<2>(snake_head);
    DBG_OUTPUT_PORT("Head at: ")
    DBG_OUTPUT_PORT(std::get<1>(snake_head))
    DBG_OUTPUT_PORT(",")
    DBG_OUTPUT_PORT_NL(std::get<2>(snake_head))
    if(dir == SNAKE_UP){
        std::get<1>(snake_head) = start-1;
        y_intervals.pop_front();
        snake.pop_front();
        snake.push_front(snake_head);
        y_intervals.push_front(snake_head);
    } else if(dir == SNAKE_DOWN) {
        std::get<2>(snake_head) = end+1;
        y_intervals.pop_front();
        snake.pop_front();
        snake.push_front(snake_head);
        y_intervals.push_front(snake_head);
    } else if(dir == SNAKE_LEFT){
        std::get<1>(snake_head) = start-1;
        x_intervals.pop_front();
        snake.pop_front();
        snake.push_front(snake_head);
        x_intervals.push_front(snake_head);
    } else if(dir == SNAKE_RIGHT) {
        std::get<2>(snake_head) = end+1;
        x_intervals.pop_front();
        snake.pop_front();
        snake.push_front(snake_head);
        x_intervals.push_front(snake_head);
    }
    DBG_OUTPUT_PORT("Head now at: ")
    DBG_OUTPUT_PORT(std::get<1>(snake_head))
    DBG_OUTPUT_PORT(",")
    DBG_OUTPUT_PORT_NL(std::get<2>(snake_head))
}

/*
updates the tail interval, should be called on every stepping
*/
void Snake::update_tail_interval() {
    DBG_OUTPUT_PORT_NL("Updating tail interval")
    IntervalType_t snake_tail = snake.back();
    DirectionType_t dir = std::get<3>(snake_tail);
    unsigned int start = std::get<1>(snake_tail);
    unsigned int end = std::get<2>(snake_tail);
    DBG_OUTPUT_PORT("tail_pos at: ")
    DBG_OUTPUT_PORT(std::get<1>(snake_tail))
    DBG_OUTPUT_PORT(",")
    DBG_OUTPUT_PORT_NL(std::get<2>(snake_tail))
    if(dir == SNAKE_UP){
        std::get<2>(snake_tail) = end-1;
        y_intervals.pop_back();
        snake.pop_back();
        snake.push_back(snake_tail);
        y_intervals.push_back(snake_tail);
    } else if(dir == SNAKE_DOWN) {
        std::get<1>(snake_tail) = start+1;
        y_intervals.pop_back();
        snake.pop_back();
        snake.push_back(snake_tail);
        y_intervals.push_back(snake_tail);
    } else if(dir == SNAKE_LEFT){
        std::get<2>(snake_tail) = end-1;
        x_intervals.pop_back();
        snake.pop_back();
        snake.push_back(snake_tail);
        x_intervals.push_back(snake_tail);
    } else if(dir == SNAKE_RIGHT) {
        std::get<1>(snake_tail) = start+1;
        x_intervals.pop_back();
        snake.pop_back();
        snake.push_back(snake_tail);
        x_intervals.push_back(snake_tail);
    }
    DBG_OUTPUT_PORT("Tail now at: ")
    DBG_OUTPUT_PORT(std::get<1>(snake_tail))
    DBG_OUTPUT_PORT(",")
    DBG_OUTPUT_PORT_NL(std::get<2>(snake_tail))
    //case the tail end moves to next interval
    if(start == end) {
        DBG_OUTPUT_PORT_NL("Tail moving to next interval")
        snake.pop_back(); //remove current tail interval pinter from snake
        if(is_horizontal_direction(dir)){
            x_intervals.pop_back(); //remove current tail interval pointer from x_intervals
        } else {
            y_intervals.pop_back(); //remove current tail interval pointer from y_intervals
        }
    }
}

/*
returns new horizontal interval created by head moving from previous_head_pos in direction dir
*/
IntervalType_t Snake::create_new_x_interval(DirectionType_t dir) {
    DBG_OUTPUT_PORT_NL("Creating new interval")
    if(dir == SNAKE_LEFT) {
        return std::make_tuple(head_pos.y, head_pos.x, previous_head_pos.x, dir);
    } else if(dir == SNAKE_RIGHT) {
        return std::make_tuple(head_pos.y, previous_head_pos.x, head_pos.x, dir);
    } else {
      DBG_OUTPUT_PORT_NL("ERROR in create_new_x_interval")
    }
}

/*
returns new vertical interval created by head moving from previous_head_pos in direction dir
*/
IntervalType_t Snake::create_new_y_interval(DirectionType_t dir) {
    DBG_OUTPUT_PORT_NL("Creating new interval")
    if(dir == SNAKE_UP) {
        return std::make_tuple(head_pos.x, head_pos.y, previous_head_pos.y, dir);
    } else if(dir == SNAKE_DOWN) {
        return std::make_tuple(head_pos.x, previous_head_pos.y, head_pos.y, dir);
    } else {
      DBG_OUTPUT_PORT_NL("ERROR in create_new_x_interval")
    }
}

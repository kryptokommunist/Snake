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

// ---- Options ----

//comment out to disable debugging information via Serial
//#define DBG

#include <list>
#include <tuple>
#include <Arduino.h>

#ifndef SNAKE_h
#define SNAKE_h

// ---- Constants/Preprocessor functions ----

#define SNAKE_STARTING_LENGTH 3

#if defined(DBG)
  #define DBG_OUTPUT_PORT(data) Serial.print(data);
  #define DBG_OUTPUT_PORT_NL(data) Serial.println(data);
#else
  #define DBG_OUTPUT_PORT(data)
  #define DBG_OUTPUT_PORT_NL(data)
#endif

// ---- Methods ----

class Snake {

public:
  /*
    END is no direction it marks that this is the end of the snake (no direction)
    FOOD similar case, marks a food item
    AUTO is telling the snake to use the autopilot finding food
    NUMBER_OF_DIRECTIONS is useful for lookup tables with enum as index
  */
  typedef enum { UP = 0, DOWN, LEFT, RIGHT, END, FOOD, AUTO } Direction;
  Snake(unsigned int screen_width, unsigned int screen_height);
  void start_game();
  bool step(Direction dir);
  bool pixel_is_set(unsigned int x, unsigned int y);
  #if defined(DBG)
    void render_frame_buffer();
  #endif
private:
  /*
    std::get<0>(interval) => x Coordinate
    std::get<1>(interval) => y Coordinate
    std::get<2>(interval) => direction of interval
  */
  typedef std::tuple<unsigned int, unsigned int, Direction> Interval;
  typedef std::list<Interval> IntervalList;
  /*
    The snake is a list of x and y dimension points ordered tail to head
    {endpoint tuple marked with direction END, tail tuple, ... , head tuple}
    the endpoint has no direction since it just marks the end, the tail is the sencond last tuple which gives you the tails direction
  */
  unsigned int screen_height;
  unsigned int screen_width;
  bool** screen_buffer;
  unsigned int food_x;
  unsigned int food_y;
  IntervalList snake;
  //contains the eaten food not appended to the tail yet
  IntervalList snake_stomach;
  //lookup table to check for opposite directions, possibly over optimizing there
  bool is_opposite[4][4] = {{false, true, false, false}, {true, false, false, false}, {false, false, false, true}, {false, false, true, false}};
  void end_game();
  bool snake_slither(Direction dir);
  void buffer_draw_point(bool val, unsigned int x, unsigned int y);
  Interval snake_head();
  Direction snake_heading();
  Interval snake_end_point();
  Interval snake_tail();
  bool snake_head_is_in_bounds();
  bool snake_collision(unsigned int x, unsigned int y);
  Direction handle_opposite_direction(Direction dir);
  Direction snake_autopilot();
  void place_food();
};

#endif

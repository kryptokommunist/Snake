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
#define DBG

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

//SNAKE_END is no direction it marks that this is the end of the snake (no direction)
typedef enum { SNAKE_UP, SNAKE_DOWN, SNAKE_LEFT, SNAKE_RIGHT, SNAKE_END, SNAKE_FOOD } DirectionType_t;

//std::get<0>(interval) => x Coordinate
//std::get<1>(interval) => y Coordinate
//std::get<2>(interval) => direction of interval
typedef std::tuple<unsigned int, unsigned int, DirectionType_t> IntervalType_t;

// ---- Methods ----

class Snake {

public:
  Snake(unsigned int screen_width, unsigned int screen_height);
  void start_game();
  bool step(DirectionType_t dir);
  bool pixel_is_set(unsigned int x, unsigned int y);
  #if defined(DBG)
    void render_frame_buffer();
  #endif
private:
  typedef struct {unsigned int x; unsigned int y;} PointType_t;
  typedef std::list<IntervalType_t> IntervalListType_t;
  unsigned int screen_height;
  unsigned int screen_width;
  bool** screen_buffer;
  unsigned int food_x;
  unsigned int food_y;
  //the snake is a list of x and y dimension intervals ordered tail to head
  //{tail tuple marked with direction SNAKE_END, ... , head tuple}
  IntervalListType_t snake;
  //contains the eaten food
  IntervalListType_t snake_stomach;
  void end_game();
  bool snake_slither(DirectionType_t dir);
  void buffer_draw_point(bool val, unsigned int x, unsigned int y);
  IntervalType_t snake_head();
  DirectionType_t snake_heading();
  IntervalType_t snake_end_point();
  IntervalType_t snake_tail();
  bool snake_head_is_in_bounds();
  bool snake_collision(unsigned int x, unsigned int y);
  void place_food();
};

#endif

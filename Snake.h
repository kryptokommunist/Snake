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

#define SNAKE_STARTING_LENGTH 5

#if defined(DBG)
  #define DBG_OUTPUT_PORT(data) Serial.print(data);
  #define DBG_OUTPUT_PORT_NL(data) Serial.println(data);
#else
  #define DBG_OUTPUT_PORT(data)
  #define DBG_OUTPUT_PORT_NL(data)
#endif

typedef enum { SNAKE_UP, SNAKE_DOWN, SNAKE_LEFT, SNAKE_RIGHT } DirectionType_t;

//std::get<0>(interval) => the fixed x/y dimension the interval is spanning over
//std::get<1>(interval) => beginning of interval (smaller than end)
//std::get<2>(interval) => end of interval (bigger than beginning)
//std::get<3>(interval) => direction of interval
typedef std::tuple<unsigned int, unsigned int, unsigned int, DirectionType_t> IntervalType_t;

// ---- Methods ----

class Snake {

public:
  Snake(unsigned int width, unsigned int height);
  void start_game();
  bool step(DirectionType_t dir);
  bool pixel_is_set(unsigned int x, unsigned int y);
private:
  typedef struct {unsigned int x; unsigned int y;} PointType_t;
  typedef std::list<IntervalType_t> IntervalListType_t;
  unsigned int screen_height;
  unsigned int screen_width;
  DirectionType_t head_heading;
  DirectionType_t tail_heading;
  PointType_t head_pos;
  PointType_t previous_head_pos;
  PointType_t tail_pos;
  IntervalListType_t x_intervals;
  IntervalListType_t y_intervals;
  //the snake is a list of x and y dimension intervals ordered tail to head
  IntervalListType_t snake;
  void end_game();
  bool snake_is_at_position(unsigned int x, unsigned int y);
  bool point_is_in_interval(IntervalType_t interval, unsigned int point_fixed_dimension, unsigned int point_interval_dimension);
  bool snake_will_be_in_bounds(DirectionType_t dir);
  bool collision_detected();
  bool is_horizontal_direction(DirectionType_t dir);
  void handle_intervals(DirectionType_t dir);
  void update_head_interval();
  void update_tail_interval();
  IntervalType_t create_new_x_interval(DirectionType_t dir);
  IntervalType_t create_new_y_interval(DirectionType_t dir);
};

#endif

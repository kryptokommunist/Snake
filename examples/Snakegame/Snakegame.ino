// - - - - -
// A demo Application for our little Arduino Library. Please copy the FLIPDOT folder into your Arduino/libraries folder to use the lib
//
//
// Copyright (C) 2016 fluepke, kryptokommunist <fabian.luepke@student.hpi.de>,<marcus.ding@student.hpi.de>
// This work is licensed under a GNU style license.
//
// Last change: kryptokommunist
//
// Documentation and samples are available at https://github.com/nerdmanufaktur/flipdot
// - - - -
#include <FLIPDOT.h>
#include "Snake.h"

FLIPDOT* board = new FLIPDOT();
Snake* game = new Snake(50,50);

void setup() {
  Serial.begin(115200);
  board->init(); //should come after wifi is connected on ESP8266
  game->start_game();

}

void loop() {
  if(game->step(SNAKE_UP)){
    game->render_frame_buffer();
  } else {
   game->start_game();
  }
    delay(2000);
}

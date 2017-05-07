// - - - - -
// A demo Application for our little Arduino Library. Please copy the Snakegame folder into your Arduino/libraries folder to use the lib
//
//
// Copyright (C) 2016 fluepke, kryptokommunist <fabian.luepke@student.hpi.de>,<marcus.ding@student.hpi.de>
// This work is licensed under a GNU style license.
//
// Last change: kryptokommunist
//
// Documentation and samples are available at https://github.com/nerdmanufaktur/flipdot
// - - - -
#include "Snake.h"

const int xPin = A0; //X attach to A0
const int yPin = D1; //Y attach to A1
const int btPin = D0; //Bt attach to digital 7
Snake* game = new Snake(50,50);
Snake::Direction dir = Snake::RIGHT;

void setup() {
  Serial.begin(115200);
  game->start_game();
  pinMode(btPin,INPUT); //set btpin as INPUT
  digitalWrite(btPin, HIGH); //and HIGH
  pinMode(yPin,INPUT); //set btpin as INPUT
  digitalWrite(yPin, HIGH); //and HIGH
}

void loop() {
  int x = analogRead(xPin);
  int y = digitalRead(yPin);
  int bt = digitalRead(btPin);
  if(x < 400){
    dir = Snake::LEFT;
  } else if (x > 900) {
    dir = Snake::RIGHT;
  } else if (y == 0) {
    dir = Snake::UP;
  } else if (bt == 0) {
    dir = Snake::DOWN;
  }
  if(game->step(dir)){
    game->render_frame_buffer();
  } else {
    dir = Snake::RIGHT;
   game->start_game();
  }
    delay(400);
}

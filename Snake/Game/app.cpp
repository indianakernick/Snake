//
//  app.cpp
//  Snake
//
//  Created by Indi Kernick on 21/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"

void App::mainLoop() {
  init();
  Time::Delta<std::chrono::milliseconds> deltaTime;
  bool running = true;
  do {
    const uint64_t delta = deltaTime.get();
    running = running && input(delta);
    running = running && update(delta);
    render(delta);
  } while (running);
  quit();
}

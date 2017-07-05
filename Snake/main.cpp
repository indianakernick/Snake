//
//  main.cpp
//  Snake
//
//  Created by Indi Kernick on 18/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "Game/app impl.hpp"
#include <Simpleton/Event/manager.hpp>

int main(int, char **) {
  evtMan = std::make_unique<Game::EventManager>(std::numeric_limits<uint64_t>::max());
  AppImpl app;
  app.mainLoop();  
  return 0;
}

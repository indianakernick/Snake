//
//  main.cpp
//  Snake
//
//  Created by Indi Kernick on 18/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "Game/app impl.hpp"

int main(int, char **) {
  AppImpl app;
  app.mainLoop<std::chrono::milliseconds>(16, 4);
  return 0;
}

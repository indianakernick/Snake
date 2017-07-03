//
//  constants.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_constants_hpp
#define game_constants_hpp

#include "types.hpp"
#include <Simpleton/Platform/window.hpp>

const Color BACK_COLOR = {127, 127, 255, 255};
const uint64_t UPDATES_PER_SECOND = 2;
const uint64_t MILLI_PER_UPDATE = 1000 / UPDATES_PER_SECOND;
const uint64_t NUM_FRAMES = 16;

const Platform::Window::Desc WINDOW_DESC = {
  "Snake",
  {768, 768},
  true
};

const Pos GAME_SIZE = {12, 12};
const glm::vec2 TILE_SIZE = {
  static_cast<float>(WINDOW_DESC.size.x) / GAME_SIZE.x,
  static_cast<float>(WINDOW_DESC.size.y) / GAME_SIZE.y
};

#endif

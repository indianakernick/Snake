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
const Color SCORE_COLOR = {255, 255, 255, 255};

const uint64_t ANIM_FPS = 30;
const uint64_t NUM_FRAMES = 16;
const uint64_t MILLI_PER_UPDATE = (1000 * NUM_FRAMES) / ANIM_FPS;

const Platform::Window::Desc WINDOW_DESC = {
  "Snake",
  {768, 768},
  true
};

const Pos GAME_SIZE = {16, 16};
const Pos TILE_SPRITE_SIZE = {16, 16};

const int FONT_SIZE = 8;

const std::string SPRITE_IMAGE_PATH = "sprites.png";
const std::string SPRITE_ATLAS_PATH = "sprites.atlas";
const std::string FONT_PATH = "Grand9K Pixel.ttf";

const size_t MAX_ITEMS = 6;

const double ITEM_SPAWN_PROB[4] = {
  1.0 / 100.0,  //Rarity::LOW
  1.0 / 1000.0, //Rarity::MEDIUM
  1.0 / 5000.0, //Rarity::HIGH
  1.0 / 10000.0 //Rarity::VERY_HIGH
};

#endif

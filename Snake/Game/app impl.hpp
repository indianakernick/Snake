//
//  app impl.hpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_app_impl_hpp
#define game_app_impl_hpp

#include <Unpacker/unpacker.hpp>
#include <Simpleton/Platform/system info.hpp>
#include "sdl app.hpp"
#include <Simpleton/Math/dir.hpp>
#include <Simpleton/Math/dir pair.hpp>
#include <Simpleton/Time/frame.hpp>
#include <deque>
#include <glm/vec4.hpp>
#include <vector>
#include <random>

using Color = glm::tvec4<uint8_t>;
using PosScalar = int;
using Pos = glm::tvec2<PosScalar>;
using ToVec = Math::ToVec<PosScalar, Math::Dir::RIGHT, Math::Dir::DOWN>;

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

class AppImpl final : public SDLApp {
public:
  AppImpl()
    : SDLApp(WINDOW_DESC, true),
      texture(nullptr, &SDL_DestroyTexture) {
    const Pos center = GAME_SIZE / 2;
    snake = {
      {center.x - 1, center.y},
      center,
      {center.x + 1, center.y},
      {center.x + 2, center.y}
    };
  }
  
private:
  Spritesheet sheet;
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
  uint64_t animProg = 0;
  std::deque<Pos> snake;
  Pos food = {0, GAME_SIZE.y / 2};
  Math::Dir currentDir = Math::Dir::LEFT;
  Math::Dir nextDir = Math::Dir::LEFT;
  bool eating = false;

  void init() override;
  bool input(uint64_t) override;
  bool update(uint64_t) override;
  void render(uint64_t) override;
  
  void setNextDir(SDL_Scancode);
  void setFoodPos();
  void renderSprite(Pos, const std::string &, double);
  
  void renderSnakeBody(const std::string &, Pos, Pos, Pos);
  void renderSnake();
  void renderFood();
};

#endif

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

class RenderManager {
public:
  RenderManager(Platform::Renderer &, const std::string &);
  
  void reset();
  
  void update(uint64_t);
  void render(const std::string &, Pos, double = 0.0);
  
private:
  Spritesheet sheet;
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
  SDL_Renderer *renderer;
  uint64_t animProg = 0;
};

class InputManager {
public:
  InputManager() = default;
  
  
};

class Rat {
public:
  explicit Rat(Pos);
  
  void reset(Pos);
  Pos getPos() const;
  void eat();
  
  void render(RenderManager &) const;
  
private:
  Pos pos;
  bool beingEaten = false;
};

class Snake {
public:
  explicit Snake(Pos);
  
  void move(Math::Dir);
  bool isEating(const Rat &) const;
  bool colliding(Pos, bool = true) const;
  void tryToEat(Rat &);
  bool isDead() const;
  
  Pos head() const;
  Pos tail() const;
  
  void update();
  void render(RenderManager &) const;
  
private:
  std::deque<Pos> positions;
  Math::Dir currentDir = Math::Dir::LEFT;
  Math::Dir nextDir = Math::Dir::LEFT;
  bool eating = false;
  bool dead = false;
};

class AppImpl final : public SDLApp {
public:
  AppImpl();
  
private:
  RenderManager renderMan;
  uint64_t animProg = 0;
  Snake snake;
  Rat rat;

  void init() override;
  bool input(uint64_t) override;
  bool update(uint64_t) override;
  void render(uint64_t) override;
  
  Pos getFoodPos() const;
  void snakeInput(SDL_Scancode);
};

#endif

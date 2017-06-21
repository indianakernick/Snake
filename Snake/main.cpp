//
//  main.cpp
//  Snake
//
//  Created by Indi Kernick on 18/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "Game/sdl app.hpp"
#include <Simpleton/Math/dir.hpp>
#include <Simpleton/Time/freq limiter.hpp>
#include <deque>
#include <glm/vec4.hpp>
#include <vector>
#include <random>

using Color = glm::tvec4<uint8_t>;
using PosScalar = int;
using Pos = glm::tvec2<PosScalar>;
using ToVec = Math::ToVec<PosScalar, Math::Dir::RIGHT, Math::Dir::DOWN>;

const Color BACK_COLOR = {0, 0, 0, 0};
const Color SNAKE_COLOR = {255, 255, 255, 255};
const Color FOOD_COLOR = {0, 255, 0, 255};

const uint64_t UPDATES_PER_SECOND = 8;

const Platform::Window::Desc WINDOW_DESC = {
  "Snake",
  {700, 700},
  true
};

const Pos GAME_SIZE = {35, 35};

class AppImpl final : public SDLApp {
public:
  AppImpl()
    : SDLApp(WINDOW_DESC, true),
      freqLimiter(Time::OP_PER_SEC, UPDATES_PER_SECOND) {
    const Pos center = GAME_SIZE / 2;
    snake = {{center.x - 1, center.y}, center, {center.x + 1, center.y}};
  }
  
private:
  Time::DeltaFreqLimiter<uint64_t> freqLimiter;
  std::deque<Pos> snake;
  Pos food;
  Math::Dir currentDir = Math::Dir::LEFT;
  Math::Dir nextDir = Math::Dir::LEFT;

  void init() override;
  bool input(uint64_t) override;
  bool update(uint64_t) override;
  void render(uint64_t) override;
  
  void setNextDir(SDL_Scancode);
  void setFoodPos();
  
  void renderSnake();
  void renderFood();
};

int main(int, char **) {
  AppImpl app;
  app.mainLoop();
  
  return 0;
}

void AppImpl::init() {
  SDL_RenderSetLogicalSize(renderer, GAME_SIZE.x, GAME_SIZE.y);
}

bool AppImpl::input(const uint64_t) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        return false;
      case SDL_KEYDOWN:
        setNextDir(e.key.keysym.scancode);
        break;
    }
  }
  return true;
}

bool AppImpl::update(const uint64_t delta) {
  freqLimiter.advance(delta);
  if (freqLimiter.canDo()) {
    snake.push_front(snake.front() + ToVec::conv(nextDir));
    snake.front() = (snake.front() + GAME_SIZE) % GAME_SIZE;
    if (snake.front() == food) {
      setFoodPos();
    } else {
      snake.pop_back();
    }
    currentDir = nextDir;
  }
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear();
  renderSnake();
  renderFood();
  renderer.present();
}

void AppImpl::setNextDir(const SDL_Scancode code) {
  switch (code) {
    case SDL_SCANCODE_UP:
      nextDir = Math::Dir::UP;
      break;
    case SDL_SCANCODE_RIGHT:
      nextDir = Math::Dir::RIGHT;
      break;
    case SDL_SCANCODE_DOWN:
      nextDir = Math::Dir::DOWN;
      break;
    case SDL_SCANCODE_LEFT:
      nextDir = Math::Dir::LEFT;
      break;
    
    default:
      nextDir = currentDir;
  }
  if (Math::opposite(nextDir) == currentDir) {
    nextDir = currentDir;
  }
}

void AppImpl::setFoodPos() {
  static std::random_device gen;
  static std::uniform_int_distribution<PosScalar> distX(0, GAME_SIZE.x - 1);
  static std::uniform_int_distribution<PosScalar> distY(0, GAME_SIZE.y - 1);
  
  auto equalsFood = [this](const Pos pos) {
    return food == pos;
  };
  do {
    food.x = distX(gen);
    food.y = distY(gen);
  } while (std::any_of(snake.cbegin(), snake.cend(), equalsFood));
}

void AppImpl::renderSnake() {
  renderer.setColor(SNAKE_COLOR);
  for (auto p = snake.cbegin(); p != snake.cend(); ++p) {
    SDL_RenderDrawPoint(renderer, p->x, p->y);
  }
}

void AppImpl::renderFood() {
  renderer.setColor(FOOD_COLOR);
  SDL_RenderDrawPoint(renderer, food.x, food.y);
}

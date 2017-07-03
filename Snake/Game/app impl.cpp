//
//  app impl.cpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app impl.hpp"

#include <random>
#include "constants.hpp"
#include <Simpleton/Platform/system info.hpp>

AppImpl::AppImpl()
  : SDLApp(WINDOW_DESC, true),
    renderMan(renderer, Platform::getResDir() + "sprites"),
    snake(GAME_SIZE / 2),
    rat({0, GAME_SIZE.y / 2}) {}

void AppImpl::init() {
  
}

bool AppImpl::input(const uint64_t) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        return false;
      case SDL_KEYDOWN:
        snakeInput(e.key.keysym.scancode);
        break;
    }
  }
  return true;
}

bool AppImpl::update(const uint64_t delta) {
  animProg += delta;
  renderMan.update(delta);
  if (animProg >= MILLI_PER_UPDATE) {
    animProg = 0;
    renderMan.reset();
    if (snake.isEating(rat)) {
      rat.reset(getFoodPos());
    }
    
    snake.update();
    snake.tryToEat(rat);
    
    if (snake.isDead()) {
      return false;
    }
  }
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear(BACK_COLOR);
  snake.render(renderMan);
  rat.render(renderMan);
  renderer.present();
}

void AppImpl::snakeInput(const SDL_Scancode code) {
  switch (code) {
    case SDL_SCANCODE_UP:
      snake.move(Math::Dir::UP);
      break;
    case SDL_SCANCODE_RIGHT:
      snake.move(Math::Dir::RIGHT);
      break;
    case SDL_SCANCODE_DOWN:
      snake.move(Math::Dir::DOWN);
      break;
    case SDL_SCANCODE_LEFT:
      snake.move(Math::Dir::LEFT);
    default: ;
  }
}

Pos AppImpl::getFoodPos() const {
  static std::random_device gen;
  static std::uniform_int_distribution<PosScalar> distX(0, GAME_SIZE.x - 1);
  static std::uniform_int_distribution<PosScalar> distY(0, GAME_SIZE.y - 1);
  
  Pos newPos;
  
  do {
    newPos = {distX(gen), distY(gen)};
  } while (snake.colliding(newPos));
  
  return newPos;
}

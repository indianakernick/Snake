//
//  app impl.cpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app impl.hpp"

#include <random>
#include "reverser.hpp"
#include "constants.hpp"
#include <Simpleton/Platform/system info.hpp>

AppImpl::AppImpl()
  : SDLApp(WINDOW_DESC, true),
    renderMan(renderer, fontLib),
    snake(GAME_SIZE / 2),
    rat({0, GAME_SIZE.y / 2}) {}

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
      score.incr();
      rat.reset(getFreePos());
    } else {
      for (auto p = powerups.begin(); p != powerups.end(); ++p) {
        if (snake.finishConsuming(**p)) {
          auto consumed = p--;
          powerups.erase(consumed);
        } else {
          snake.tryToConsume(**p);
        }
      }
    }
    
    if (shouldSpawnPowerUp()) {
      powerups.emplace_back(makeReverser(getFreePos()));
    }
    
    snake.update();
    snake.tryToEat(rat);
    
    if (snake.isDead()) {
      score.reset();
      return false;
    }
  }
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear(BACK_COLOR);
  for (auto p = powerups.cbegin(); p != powerups.cend(); ++p) {
    (*p)->render(renderMan);
  }
  rat.render(renderMan);
  snake.render(renderMan);
  score.render(renderMan);
  renderer.present();
}

bool AppImpl::shouldSpawnPowerUp() const {
  static std::random_device gen;
  std::uniform_int_distribution<int> dist(0, POWERUP_SPAWN_PROB);
  
  return dist(gen) == 0 && powerups.size() < MAX_POWERUPS;
}

Pos AppImpl::getFreePos() const {
  static std::random_device gen;
  std::uniform_int_distribution<PosScalar> distX(0, GAME_SIZE.x - 1);
  std::uniform_int_distribution<PosScalar> distY(0, GAME_SIZE.y - 1);
  
  Pos newPos;
  
  bool overlapping = true;
  while (overlapping) {
    overlapping = false;
  
    newPos = {distX(gen), distY(gen)};
    if (snake.colliding(newPos)) {
      overlapping = true;
      continue;
    }
    
    for (auto p = powerups.cbegin(); p != powerups.cend(); ++p) {
      if (newPos == (*p)->getPos()) {
        overlapping = true;
        break;
      }
    }
  }
  
  return newPos;
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

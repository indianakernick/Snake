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
    rat({0, GAME_SIZE.y / 2}) {
  itemFactories.emplace_back(&makeReverser);
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
      score.incr();
      rat = Rat(getFreePos());
    } else {
      for (auto i = items.cbegin(); i != items.cend(); ++i) {
        if (snake.tryToConsume(**i)) {
          auto consumed = i--;
          items.erase(consumed);
        }
      }
      
      rat.update();
    }
    
    for (auto i = items.cbegin(); i != items.cend(); ++i) {
      (*i)->update();
    }
    
    if (shouldSpawnItem()) {
      spawnItem();
    }
    
    snake.update();
    snake.tryToConsume(rat);
    
    if (snake.isDead()) {
      score.reset();
      return false;
    }
  }
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear(BACK_COLOR);
  for (auto p = items.cbegin(); p != items.cend(); ++p) {
    (*p)->render(renderMan);
  }
  rat.render(renderMan);
  snake.render(renderMan);
  score.render(renderMan);
  renderer.present();
}

void AppImpl::spawnItem() {
  static std::random_device gen;
  std::uniform_int_distribution<size_t> dist(0, itemFactories.size() - 1);
  
  items.emplace_back(itemFactories[dist(gen)](getFreePos()));
}

bool AppImpl::shouldSpawnItem() const {
  static std::random_device gen;
  std::uniform_int_distribution<int> dist(0, POWERUP_SPAWN_PROB);
  
  return dist(gen) == 0 && items.size() < MAX_POWERUPS;
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
    
    for (auto p = items.cbegin(); p != items.cend(); ++p) {
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

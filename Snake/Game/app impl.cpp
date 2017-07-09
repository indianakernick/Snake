//
//  app impl.cpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app impl.hpp"

#include <random>
#include "coin.hpp"
#include "slicer.hpp"
#include "reverser.hpp"
#include "constants.hpp"
#include "invis potion.hpp"
#include <Simpleton/Event/manager.hpp>
#include <Simpleton/Platform/system info.hpp>

bool AppImpl::init() {
  SDLApp::initWindow(WINDOW_DESC, true);
  renderMan.init(renderer);
  evtMan = std::make_unique<Game::EventManager>();
  score.init();
  
  reset();
  itemFactories.emplace_back(&makeItem<Reverser>);
  itemFactories.emplace_back(&makeItem<Slicer>);
  itemFactories.emplace_back(&makeItem<Coin>);
  itemFactories.emplace_back(&makeItem<InvisPotion>);
  itemProbs.emplace_back(ITEM_SPAWN_PROB[static_cast<size_t>(Reverser::RARITY)]);
  itemProbs.emplace_back(ITEM_SPAWN_PROB[static_cast<size_t>(Slicer::RARITY)]);
  itemProbs.emplace_back(ITEM_SPAWN_PROB[static_cast<size_t>(Coin::RARITY)]);
  itemProbs.emplace_back(ITEM_SPAWN_PROB[static_cast<size_t>(InvisPotion::RARITY)]);
  
  return RUN;
}

void AppImpl::quit() {
  score.quit();
  evtMan.reset();
  renderMan.quit();
  SDLApp::quitWindow();
}

bool AppImpl::input(const uint64_t) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    switch (e.type) {
      case SDL_QUIT:
        return QUIT;
      case SDL_KEYDOWN:
        if (state == State::GAME) {
          snakeInput(e.key.keysym.scancode);
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
          changeState();
        }
        break;
    }
  }
  return RUN;
}

bool AppImpl::update(const uint64_t delta) {
  if (state == State::GAME) {
    animProg += delta;
    renderMan.update(delta);
    if (animProg >= MILLI_PER_UPDATE) {
      animProg = 0;
      renderMan.reset();
      
      updateGame();
    }
  }
  return RUN;
}

void renderTitle(RenderManager &renderMan, const char *title, const char *subTitle) {
  const Pos titleSize = renderMan.textSize(TITLE_FONT_SIZE, title);
  const Pos titlePos = (GAME_SIZE * TILE_SPRITE_SIZE - titleSize) / 2;
  renderMan.renderText(TITLE_FONT_SIZE, title, TEXT_COLOR, titlePos);
  
  const PosScalar subTitleWidth = renderMan.textSize(SUBTITLE_FONT_SIZE, subTitle).x;
  const Pos subTitlePos = {(GAME_SIZE.x * TILE_SPRITE_SIZE.x - subTitleWidth) / 2, titlePos.y + titleSize.y};
  renderMan.renderText(SUBTITLE_FONT_SIZE, subTitle, TEXT_COLOR, subTitlePos);
}

void AppImpl::render(const uint64_t) {
  renderer.clear();
  renderMan.renderBack();
  if (state != State::HOME) {
    for (auto p = items.cbegin(); p != items.cend(); ++p) {
      (*p)->render(renderMan);
    }
    rat.render(renderMan);
    snake.render(renderMan);
    score.render(renderMan);
  }
  
  if (state == State::HOME) {
    renderTitle(renderMan, "Snake", "Press space to play");
  } else if (state == State::PAUSE) {
    renderTitle(renderMan, "Pause", "Press space to resume");
  } else if (state == State::DEAD) {
    renderTitle(renderMan, "Wasted", "Press space to play again");
  }
  
  renderer.present();
}

void AppImpl::updateGame() {
  bool shouldSpawnRat = false;
  if (snake.isEating(rat)) {
    score.incr();
    shouldSpawnRat = true;
  } else {
    for (auto i = items.cbegin(); i != items.cend(); ++i) {
      if ((*i)->isDespawning() || snake.tryToConsume(**i)) {
        auto consumed = i--;
        items.erase(consumed);
      }
    }
    
    rat.update();
  }
  
  for (auto i = items.cbegin(); i != items.cend(); ++i) {
    (*i)->update();
  }
  
  evtMan->update();
  
  spawnItemIfShould();
  
  snake.update();
  if (shouldSpawnRat) {
    rat = Rat(getFreePos());
  }
  snake.tryToConsume(rat);
  
  if (snake.isDead()) {
    score.reset();
    state = State::DEAD;
  }
}

void AppImpl::spawnItemIfShould() {
  static std::random_device gen;
  
  if (items.size() >= MAX_ITEMS) {
    return;
  }
  
  std::uniform_real_distribution<double> dist(0.0, 1.0);
  double num = dist(gen);
  
  for (size_t i = 0; i != itemProbs.size(); i++) {
    if (num < itemProbs[i]) {
      items.emplace_back(itemFactories[i](getFreePos()));
      return;
    } else {
      num -= itemProbs[i];
    }
  }
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
    
    if (newPos == rat.getPos()) {
      overlapping = true;
      continue;
    }
    
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

void AppImpl::changeState() {
  switch (state) {
    case State::HOME:
      state = State::GAME;
      reset();
      break;
    case State::GAME:
      state = State::PAUSE;
      break;
    case State::PAUSE:
      state = State::GAME;
      break;
    case State::DEAD:
      state = State::GAME;
      reset();
  }
}

void AppImpl::reset() {
  snake = Snake(GAME_SIZE / 2);
  rat = Rat({0, GAME_SIZE.y / 2});
  items.clear();
  score.reset();
}

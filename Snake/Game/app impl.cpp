//
//  app impl.cpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app impl.hpp"

RenderManager::RenderManager(Platform::Renderer &renderer, const std::string &sheetPath)
  : sheet(makeSpritesheet(sheetPath + ".atlas", sheetPath + ".png")),
    texture(nullptr, &SDL_DestroyTexture),
    renderer(renderer.get()) {
  const Image &image = sheet.getImage();
  texture.reset(SDL_CreateTexture(
    renderer.get(),
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STATIC,
    image.width(),
    image.height()
  ));
  SDL_UpdateTexture(texture.get(), nullptr, image.data(), static_cast<int>(image.pitch()));
  SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
}

void RenderManager::reset() {
  animProg = 0;
}

void RenderManager::update(const uint64_t delta) {
  animProg += delta;
}

SDL_Rect toSDL(const RectPx rect) {
  return {rect.x, rect.y, rect.w, rect.h};
}

SDL_Rect toTile(const Pos pos) {
  return {
    static_cast<int>(pos.x * TILE_SIZE.x),
    static_cast<int>(pos.y * TILE_SIZE.y),
    static_cast<int>(TILE_SIZE.x),
    static_cast<int>(TILE_SIZE.y)
  };
}

void RenderManager::render(const std::string &name, const Pos pos, const double rotation) {
  const double prog = static_cast<double>(animProg) / MILLI_PER_UPDATE;
  const std::string frame = std::to_string(Time::progToFrame(prog, NUM_FRAMES));
  const SDL_Rect srcRect = toSDL(sheet.getSprite(name + ' ' + frame));
  const SDL_Rect dstRect = toTile(pos);
  SDL_RenderCopyEx(
    renderer,
    texture.get(),
    &srcRect,
    &dstRect,
    rotation,
    nullptr,
    SDL_FLIP_NONE
  );
}

Rat::Rat(const Pos pos)
  : pos(pos) {}

void Rat::reset(const Pos newPos) {
  pos = newPos;
  beingEaten = false;
}

Pos Rat::getPos() const {
  return pos;
}

void Rat::eat() {
  beingEaten = true;
}

void Rat::render(RenderManager &renderer) const {
  if (not beingEaten) {
    renderer.render("rat", pos);
  }
}

Snake::Snake(const Pos center)
  : positions({
      {center.x - 1, center.y},
      center,
      {center.x + 1, center.y},
      {center.x + 2, center.y}
    }) {}

void Snake::move(const Math::Dir newDir) {
  if (Math::opposite(newDir) != currentDir) {
    nextDir = newDir;
  }
}

bool Snake::isEating(const Rat &rat) const {
  return eating && positions.front() == rat.getPos();
}

bool Snake::colliding(const Pos pos, const bool withHead) const {
  return std::any_of(
    positions.cbegin() + !withHead,
    positions.cend(),
    [pos] (const Pos thisPos) {
      return pos == thisPos;
    }
  );
}

void Snake::tryToEat(Rat &rat) {
  if (head() == rat.getPos()) {
    eating = true;
    rat.eat();
  }
}

bool Snake::isDead() const {
  return dead;
}

Pos Snake::head() const {
  return positions.front();
}

Pos Snake::tail() const {
  return positions.back();
}

void Snake::update() {
  if (eating) {
    eating = false;
  } else {
    positions.pop_back();
  }
  positions.push_front((positions.front() + ToVec::conv(nextDir) + GAME_SIZE) % GAME_SIZE);
  currentDir = nextDir;
  if (colliding(head(), false) && head() != tail()) {
    dead = true;
  }
}

//returns a vector pointing from this to prev
Pos getDirVec(const Pos thisPos, const Pos prevPos) {
  Pos prevToThis = thisPos - prevPos;
  
  //this accounts for the snake going into one side of the window and
  //and coming out from the opposite side of the window
  if (prevToThis.x > 1) {
    prevToThis.x -= GAME_SIZE.x;
  }
  if (prevToThis.y > 1) {
    prevToThis.y -= GAME_SIZE.y;
  }
  if (prevToThis.x < -1) {
    prevToThis.x += GAME_SIZE.x;
  }
  if (prevToThis.y < -1) {
    prevToThis.y += GAME_SIZE.y;
  }
  
  return prevToThis;
}

double getBodySpriteAngle(const Math::Dir prevToThisDir) {
  return Math::ToNum<double>::conv(prevToThisDir, 90.0);
}

std::string getBodySpriteTurnName(const Math::Dir prevToThisDir, const Math::Dir thisToNextDir) {
  const Math::SignedDirType diff = Math::diff(prevToThisDir, thisToNextDir);
  if (diff == 0) {
    return "straight";
  } else if (diff == 1) {
    return "right corner";
  } else if (diff == -1) {
    return "left corner";
  } else {
    assert(false);
    return "";
  }
}

void renderBody(
  RenderManager &renderer,
  const std::string &name,
  const Pos pos,
  const Pos prevToThisVec,
  const Pos thisToNextVec
) {
  using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  const Math::Dir prevToThisDir = FromVec::conv(prevToThisVec);
  const Math::Dir thisToNextDir = FromVec::conv(thisToNextVec);
  renderer.render(
    name + ' ' + getBodySpriteTurnName(prevToThisDir, thisToNextDir),
    pos,
    getBodySpriteAngle(prevToThisDir)
  );
}

std::string dirToString(const Math::Dir dir) {
  static const std::string NAMES[4] = {
    "up", "right", "down", "left"
  };
  return NAMES[Math::ToNum<size_t>::conv(dir)];
}

void Snake::render(RenderManager &renderer) const {
  using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  if (eating) {
    renderer.render("eat rat " + dirToString(currentDir), head());
  } else {
    renderer.render("head", head(), getBodySpriteAngle(currentDir));
  }

  Pos nextPos = positions[2];
  Pos thisToNextVec = getDirVec(positions[1], nextPos);
  renderBody(renderer, "head", positions[1], thisToNextVec, getDirVec(positions[0], positions[1]));
  
  for (auto b = positions.cbegin() + 2; b != positions.cend() - 2; ++b) {
    const Pos pos = nextPos;
    nextPos = *(b + 1);
    const Pos prevToThisVec = getDirVec(pos, nextPos);
    renderBody(renderer, "body", pos, prevToThisVec, thisToNextVec);
    thisToNextVec = prevToThisVec;
  }
  
  const Pos prevToThisVec = getDirVec(nextPos, positions.back());
  renderBody(renderer, eating ? "body" : "tail", nextPos, prevToThisVec, thisToNextVec);
  thisToNextVec = prevToThisVec;

  const double tailAngle = getBodySpriteAngle(FromVec::conv(thisToNextVec));
  renderer.render(eating ? "tail grow" : "tail", tail(), tailAngle);
}

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

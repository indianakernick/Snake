//
//  main.cpp
//  Snake
//
//  Created by Indi Kernick on 18/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <Unpacker/unpacker.hpp>
#include <Simpleton/Platform/system info.hpp>
#include "Game/sdl app.hpp"
#include <Simpleton/Math/dir.hpp>
#include <Simpleton/Math/dir pair.hpp>
#include <Simpleton/Time/freq limiter.hpp>
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

int main(int, char **) {
  AppImpl app;
  app.mainLoop();
  
  return 0;
}

void AppImpl::init() {
  const std::string atlasPath = Platform::getResDir() + "sprites.atlas";
  const std::string imagePath = Platform::getResDir() + "sprites.png";
  sheet = makeSpritesheet(atlasPath, imagePath);
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
  animProg += delta;
  if (animProg >= MILLI_PER_UPDATE) {
    animProg = 0;
    if (eating) {
      setFoodPos();
      eating = false;
    } else {
      snake.pop_back();
    }
    
    snake.push_front((snake.front() + ToVec::conv(nextDir) + GAME_SIZE) % GAME_SIZE);
    
    if (snake.front() != food) {
      for (auto s = snake.cbegin() + 1; s != snake.cend() - 1; ++s) {
        if (*s == snake.front()) {
          return false;
        }
      }
    } else {
      eating = true;
    }
    currentDir = nextDir;
  }
  return true;
}

void AppImpl::render(const uint64_t) {
  renderer.clear(BACK_COLOR);
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
    food = {distX(gen), distY(gen)};
  } while (std::any_of(snake.cbegin(), snake.cend(), equalsFood));
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

void AppImpl::renderSprite(
  const Pos pos,
  const std::string &name,
  const double rotation = 0.0
) {
  const double prog = static_cast<double>(animProg) / MILLI_PER_UPDATE;
  const std::string frame = std::to_string(Time::progToFrame(prog, NUM_FRAMES));
  const SDL_Rect srcRect = toSDL(sheet.getSprite(name + ' ' + frame));
  const SDL_Rect dstRect = toTile(pos);
  SDL_RenderCopyEx(
    renderer.get(),
    texture.get(),
    &srcRect,
    &dstRect,
    rotation,
    nullptr,
    SDL_FLIP_NONE
  );
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

void AppImpl::renderSnakeBody(
  const std::string &name,
  const Pos pos,
  const Pos prevToThisVec,
  const Pos thisToNextVec
) {
  using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  const Math::Dir prevToThisDir = FromVec::conv(prevToThisVec);
  const Math::Dir thisToNextDir = FromVec::conv(thisToNextVec);
  renderSprite(
    pos,
    name + ' ' + getBodySpriteTurnName(prevToThisDir, thisToNextDir),
    getBodySpriteAngle(prevToThisDir)
  );
}

std::string dirToString(const Math::Dir dir) {
  static const std::string NAMES[4] = {
    "up", "right", "down", "left"
  };
  return NAMES[Math::ToNum<size_t>::conv(dir)];
}

void AppImpl::renderSnake() {
  using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  if (eating) {
    renderSprite(snake.front(), "eat rat " + dirToString(currentDir));
  } else {
    renderSprite(snake.front(), "head", getBodySpriteAngle(currentDir));
  }

  Pos nextPos = snake[2];
  Pos thisToNextVec = getDirVec(snake[1], nextPos);
  renderSnakeBody("head", snake[1], thisToNextVec, getDirVec(snake[0], snake[1]));
  
  for (auto b = snake.cbegin() + 2; b != snake.cend() - 2; ++b) {
    const Pos pos = nextPos;
    nextPos = *(b + 1);
    const Pos prevToThisVec = getDirVec(pos, nextPos);
    renderSnakeBody("body", pos, prevToThisVec, thisToNextVec);
    thisToNextVec = prevToThisVec;
  }
  
  const Pos prevToThisVec = getDirVec(nextPos, snake.back());
  renderSnakeBody(eating ? "body" : "tail", nextPos, prevToThisVec, thisToNextVec);
  thisToNextVec = prevToThisVec;

  const double tailAngle = getBodySpriteAngle(FromVec::conv(thisToNextVec));
  renderSprite(snake.back(), eating ? "tail grow" : "tail", tailAngle);
}

void AppImpl::renderFood() {
  if (not eating) {
    renderSprite(food, "rat");
  }
}

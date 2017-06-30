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

const Platform::Window::Desc WINDOW_DESC = {
  "Snake",
  {768, 768},
  true
};

const Pos GAME_SIZE = {6, 6};
const glm::vec2 TILE_SIZE = {
  static_cast<float>(WINDOW_DESC.size.x) / GAME_SIZE.x,
  static_cast<float>(WINDOW_DESC.size.y) / GAME_SIZE.y
};

class AppImpl final : public SDLApp {
public:
  AppImpl()
    : SDLApp(WINDOW_DESC, true),
      texture(nullptr, &SDL_DestroyTexture),
      freqLimiter(Time::OP_PER_SEC, UPDATES_PER_SECOND) {
    const Pos center = GAME_SIZE / 2;
    snake = {{center.x - 1, center.y}, center, {center.x + 1, center.y}};
  }
  
private:
  Spritesheet sheet;
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
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
  void renderSprite(Pos, const std::string &);
  void renderSprite(Pos, const std::string &, double);
  
  void renderSnakeBody(Pos, Pos, Pos);
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
  freqLimiter.advance(delta);
  if (freqLimiter.canDo()) {
    snake.push_front(snake.front() + ToVec::conv(nextDir));
    snake.front() = (snake.front() + GAME_SIZE) % GAME_SIZE;
    
    if (snake.front() == food) {
      //food is never is the same position as the snake
      setFoodPos();
    } else {
      snake.pop_back();
      for (auto s = snake.cbegin() + 1; s != snake.cend(); ++s) {
        if (*s == snake.front()) {
          return false;
        }
      }
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

void AppImpl::renderSprite(const Pos pos, const std::string &name) {
  const SDL_Rect srcRect = toSDL(sheet.getSprite(name));
  const SDL_Rect dstRect = toTile(pos);
  SDL_RenderCopy(
    renderer.get(),
    texture.get(),
    &srcRect,
    &dstRect
  );
}

void AppImpl::renderSprite(const Pos pos, const std::string &name, const double rotation) {
  const SDL_Rect srcRect = toSDL(sheet.getSprite(name));
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

//returns a vector pointing from this to behind
Pos getDirVec(const Pos thisPos, const Pos behindPos) {
  Pos behindToThis = thisPos - behindPos;
  
  //this accounts for the snake going into one side of the window and
  //and coming out from the opposite side of the window
  if (behindToThis.x > 1) {
    behindToThis.x -= GAME_SIZE.x;
  }
  if (behindToThis.y > 1) {
    behindToThis.y -= GAME_SIZE.y;
  }
  if (behindToThis.x < -1) {
    behindToThis.x += GAME_SIZE.x;
  }
  if (behindToThis.y < -1) {
    behindToThis.y += GAME_SIZE.y;
  }
  
  return behindToThis;
}

void AppImpl::renderSnakeBody(const Pos pos, const Pos behindVec, const Pos frontVec) {
  using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  const Math::Dir behindDir = FromVec::conv(behindVec);
  const Math::Dir frontDir = FromVec::conv(frontVec);
  const Math::DirPair behindCurrent = Math::makePair(behindDir, frontDir);
  
  switch (behindCurrent) {
    case Math::DirPair::UP_UP:
      renderSprite(pos, "straight");
      break;
    case Math::DirPair::UP_RIGHT:
      renderSprite(pos, "right corner");
      break;
    case Math::DirPair::UP_DOWN:
      assert(false);
      break;
    case Math::DirPair::UP_LEFT:
      renderSprite(pos, "left corner");
      break;
    
    case Math::DirPair::RIGHT_UP:
      renderSprite(pos, "left corner", 90.0);
      break;
    case Math::DirPair::RIGHT_RIGHT:
      renderSprite(pos, "straight", 90.0);
      break;
    case Math::DirPair::RIGHT_DOWN:
      renderSprite(pos, "right corner", 90.0);
      break;
    case Math::DirPair::RIGHT_LEFT:
      assert(false);
      break;
    
    case Math::DirPair::DOWN_UP:
      assert(false);
      break;
    case Math::DirPair::DOWN_RIGHT:
      renderSprite(pos, "left corner", 180.0);
      break;
    case Math::DirPair::DOWN_DOWN:
      renderSprite(pos, "straight", 180.0);
      break;
    case Math::DirPair::DOWN_LEFT:
      renderSprite(pos, "right corner", 180.0);
      break;
    
    case Math::DirPair::LEFT_UP:
      renderSprite(pos, "right corner", 270.0);
      break;
    case Math::DirPair::LEFT_RIGHT:
      assert(false);
      break;
    case Math::DirPair::LEFT_DOWN:
      renderSprite(pos, "left corner", 270.0);
      break;
    case Math::DirPair::LEFT_LEFT:
      renderSprite(pos, "straight", 270.0);
      break;
    
    default:
      assert(false);
  }
}

void AppImpl::renderSnake() {
  using ToAngle = Math::ToNum<double>;
  using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  renderSprite(snake.front(), "head", ToAngle::conv(currentDir, 90.0));

  Pos front = getDirVec(snake.front(), snake[1]);
  Pos nextPos = snake[1];
  
  for (auto b = snake.cbegin() + 1; b != snake.cend() - 1; ++b) {
    const Pos pos = nextPos;
    nextPos = *(b + 1);
    const Pos back = getDirVec(pos, nextPos);
    renderSnakeBody(pos, back, front);
    front = back;
  }
  
  const Math::Dir backDir = FromVec::conv(getDirVec(*(snake.cend() - 2), snake.back()));

  renderSprite(snake.back(), "tail", ToAngle::conv(backDir, 90.0));
}

void AppImpl::renderFood() {
  renderSprite(food, "food");
}

//
//  main.cpp
//  Snake
//
//  Created by Indi Kernick on 18/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <SDL2/SDL.h>
#include "Engine/Math/dir.hpp"
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

const size_t UPDATES_PER_SECOND = 8;
const size_t FRAMES_PER_UPDATE = 60 / UPDATES_PER_SECOND;

const Pos GAME_SIZE = {35, 35};
const Pos WINDOW_SIZE = {700, 700};
const glm::tvec2<float> PX_SIZE = {
  static_cast<float>(WINDOW_SIZE.x) / GAME_SIZE.x,
  static_cast<float>(WINDOW_SIZE.y) / GAME_SIZE.y
};

void setColor(SDL_Renderer *, Color);
Math::Dir getNextDir(SDL_Scancode, Math::Dir);
Pos getFoodPos(const std::deque<Pos> &);
void renderSnake(SDL_Renderer *, const std::deque<glm::ivec2> &);
void renderFood(SDL_Renderer *, Pos);

int main(int, char **) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Window *window = SDL_CreateWindow(
    "Snake",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    WINDOW_SIZE.x, WINDOW_SIZE.y,
    SDL_WINDOW_SHOWN
  );
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  
  Math::Dir currentDir = Math::Dir::LEFT;
  Math::Dir nextDir = currentDir;
  const Pos center = GAME_SIZE / 2;
  std::deque<Pos> snake = {{center.x - 1, center.y}, center, {center.x + 1, center.y}};
  size_t frame = 0;
  bool quit = false;
  Pos food = getFoodPos(snake);
  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          nextDir = getNextDir(e.key.keysym.scancode, currentDir);
          break;
      }
    }
    
    if (frame % FRAMES_PER_UPDATE == 0) {
      snake.push_front(snake.front() + ToVec::conv(nextDir));
      snake.front() = (snake.front() + GAME_SIZE) % GAME_SIZE;
      if (snake.front() == food) {
        food = getFoodPos(snake);
      } else {
        snake.pop_back();
      }
      currentDir = nextDir;
    }
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    renderSnake(renderer, snake);
    renderFood(renderer, food);
    SDL_RenderPresent(renderer);
    
    frame++;
  }
  
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}

void setColor(SDL_Renderer *renderer, const Color color) {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

Math::Dir getNextDir(const SDL_Scancode code, const Math::Dir currentDir) {
  Math::Dir nextDir = currentDir;
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
    
    default: ;
  }
  if (Math::opposite(nextDir) == currentDir) {
    return currentDir;
  } else {
    return nextDir;
  }
}

Pos getFoodPos(const std::deque<Pos> &snake) {
  static std::random_device gen;
  static std::uniform_int_distribution<PosScalar> distX(0, GAME_SIZE.x - 1);
  static std::uniform_int_distribution<PosScalar> distY(0, GAME_SIZE.y - 1);
  
  Pos food;
  auto equalsFood = [&food](const Pos pos) {
    return food == pos;
  };
  do {
    food.x = distX(gen);
    food.y = distY(gen);
  } while (std::any_of(snake.cbegin(), snake.cend(), equalsFood));
  
  return food;
}

SDL_Rect scalePx(const Pos pos) {
  return {
    static_cast<int>(pos.x * PX_SIZE.x),
    static_cast<int>(pos.y * PX_SIZE.y),
    static_cast<int>(PX_SIZE.x),
    static_cast<int>(PX_SIZE.y)
  };
}

void renderSnake(SDL_Renderer *renderer, const std::deque<glm::ivec2> &snake) {
  setColor(renderer, SNAKE_COLOR);
  
  std::vector<SDL_Rect> rects;
  for (auto p = snake.cbegin(); p != snake.cend(); ++p) {
    rects.push_back(scalePx(*p));
  }
  
  SDL_RenderFillRects(renderer, rects.data(), static_cast<int>(rects.size()));
}

void renderFood(SDL_Renderer *renderer, const Pos food) {
  setColor(renderer, FOOD_COLOR);
  
  const SDL_Rect rect = scalePx(food);
  SDL_RenderFillRect(renderer, &rect);
}

//
//  render manager.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "render manager.hpp"

#include "constants.hpp"
#include <Surface/load.hpp>
#include <Simpleton/SDL/paths.hpp>
#include <Simpleton/Time/frame.hpp>
#include <Simpleton/SDL/surface.hpp>

void RenderManager::init(SDL::Renderer &otherRenderer) {
  sheet = Unpack::makeSpritesheet(SDL::getResDir() + SPRITE_ATLAS_PATH);
  renderer = otherRenderer.get();
  SDL_RenderSetLogicalSize(renderer, GAME_SIZE.x * TILE_SPRITE_SIZE.x, GAME_SIZE.y * TILE_SPRITE_SIZE.y);
  
  const Surface image = loadSurface(SDL::getResDir() + SPRITE_IMAGE_PATH);
  texture.reset(SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STATIC,
    image.width(),
    image.height()
  ));
  SDL_UpdateTexture(texture.get(), nullptr, image.data(), image.pitch());
  SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
}

void RenderManager::quit() {
  texture.reset();
  fonts.clear();
}

void RenderManager::reset() {
  animProg = 0;
}

void RenderManager::update(const uint64_t delta) {
  animProg += delta;
}

void RenderManager::renderBack() {
  SDL_SetRenderDrawColor(renderer, BACK_COLOR.r, BACK_COLOR.g, BACK_COLOR.b, BACK_COLOR.a);
  const SDL_Rect rect = {0, 0, GAME_SIZE.x * TILE_SPRITE_SIZE.x, GAME_SIZE.y * TILE_SPRITE_SIZE.y};
  SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect toSDL(const Unpack::RectPx rect) {
  return {rect.x, rect.y, rect.w, rect.h};
}

SDL_Rect toTile(const Pos pos) {
  return {
    static_cast<int>(pos.x * TILE_SPRITE_SIZE.x),
    static_cast<int>(pos.y * TILE_SPRITE_SIZE.y),
    static_cast<int>(TILE_SPRITE_SIZE.x),
    static_cast<int>(TILE_SPRITE_SIZE.y)
  };
}

void RenderManager::renderTile(const std::string &name, const Pos pos, const double rotation) {
  using ProgToFrame = Time::ProgToFrame<uint32_t>;

  const double prog = static_cast<double>(animProg) / MILLI_PER_UPDATE;
  const std::string frame = std::to_string(ProgToFrame::conv(prog, NUM_FRAMES));
  renderTileImpl(name + ' ' + frame, pos, rotation);
}

void RenderManager::renderTileReverse(const std::string &name, const Pos pos, const double rotation) {
  using ProgToFrame = Time::ProgToFrame<uint32_t, Time::Round::FLOOR, false, true>;
  
  const double prog = static_cast<double>(animProg) / MILLI_PER_UPDATE;
  const std::string frame = std::to_string(ProgToFrame::conv(prog, NUM_FRAMES));
  renderTileImpl(name + ' ' + frame, pos, rotation);
}

SDL_Color toSDL(const Color color) {
  return {color.r, color.g, color.b, color.a};
}

void RenderManager::renderText(const int size, const std::string &text, const Color color, const Pos pos) {
  TTF_Font *const font = getFont(size);
  
  SDL::Surface surface(TTF_RenderText_Solid(font, text.c_str(), toSDL(color)));
  
  if (surface == nullptr) {
    throw std::runtime_error(std::string("Failed to render text: ") + TTF_GetError());
  }
  
  SDL::Texture texture(SDL_CreateTextureFromSurface(renderer, surface.get()));
  
  if (texture == nullptr) {
    throw std::runtime_error(std::string("Failed to render text: ") + SDL_GetError());
  }
  
  SDL_Rect dstRect;
  dstRect.x = pos.x;
  dstRect.y = pos.y;
  if (SDL_QueryTexture(texture.get(), nullptr, nullptr, &dstRect.w, &dstRect.h) == -1) {
    throw std::runtime_error(std::string("Failed to render text: ") + SDL_GetError());
  }
  
  SDL_RenderCopy(renderer, texture.get(), nullptr, &dstRect);
}

Pos RenderManager::textSize(const int size, const std::string &text) {
  static_assert(std::is_same<PosScalar, int>::value);
  TTF_Font *const font = getFont(size);
  Pos textSize;
  if (TTF_SizeText(font, text.c_str(), &textSize.x, &textSize.y) == -1) {
    throw std::runtime_error(std::string("Failed to get text size: ") + TTF_GetError());
  }
  return textSize;
}

void RenderManager::renderTileImpl(
  const std::string &name,
  const Pos pos,
  const double rotation
) {
  const SDL_Rect srcRect = toSDL(sheet.getSprite(sheet.getIDfromName(name)));
  const SDL_Rect dstRect = toTile(pos);
  if (SDL_RenderCopyEx(
    renderer,
    texture.get(),
    &srcRect,
    &dstRect,
    rotation,
    nullptr,
    SDL_FLIP_NONE
  ) != 0) {
    throw std::runtime_error(std::string("Failed to render tile: ") + SDL_GetError());
  }
}

TTF_Font *RenderManager::getFont(const int size) {
  auto iter = fonts.find(size);
  if (iter == fonts.end()) {
    SDL::Font font = SDL::openFont(SDL::getResDir() + FONT_PATH, size);
    TTF_Font *const fontPtr = font.get();
    fonts.emplace(size, std::move(font));
    return fontPtr;
  } else {
    return iter->second.get();
  }
}

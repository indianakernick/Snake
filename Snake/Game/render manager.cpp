//
//  render manager.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "render manager.hpp"

#include "constants.hpp"
#include <Simpleton/Time/frame.hpp>
#include <Simpleton/Platform/sdl object.hpp>
#include <Simpleton/Platform/system info.hpp>

Spritesheet makeSheet() {
  return makeSpritesheet(
    Platform::getResDir() + SPRITE_ATLAS_PATH,
    Platform::getResDir() + SPRITE_IMAGE_PATH
  );
}

RenderManager::RenderManager(Platform::Renderer &renderer, Platform::FontLibrary &fontLib)
  : sheet(makeSheet()),
    texture(nullptr, &SDL_DestroyTexture),
    font(fontLib.openFont(Platform::getResDir() + FONT_PATH, FONT_SIZE)),
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
  
  TTF_SetFontHinting(font.get(), TTF_HINTING_NORMAL);
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

void RenderManager::renderTile(const std::string &name, const Pos pos, const double rotation) {
  const double prog = static_cast<double>(animProg) / MILLI_PER_UPDATE;
  const std::string frame = std::to_string(Time::progToFrame(prog, NUM_FRAMES));
  const SDL_Rect srcRect = toSDL(sheet.getSprite(name + ' ' + frame));
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

SDL_Color toSDL(const Color color) {
  return {color.r, color.g, color.b, color.a};
}

void RenderManager::renderText(const std::string &text, const Color color, const Pos pos) {
  SDL_OBJECT_FREE(Surface) surface(
    TTF_RenderText_Solid(font.get(), text.c_str(), toSDL(color)),
    &SDL_FreeSurface
  );
  
  if (surface == nullptr) {
    throw std::runtime_error(std::string("Failed to render text: ") + TTF_GetError());
  }
  
  SDL_OBJECT_DESTROY(Texture) texture(
    SDL_CreateTextureFromSurface(renderer, surface.get()),
    &SDL_DestroyTexture
  );
  
  if (texture == nullptr) {
    throw std::runtime_error(std::string("Failed to render text: ") + SDL_GetError());
  }
  
  SDL_Rect dstRect;
  dstRect.x = pos.x;
  dstRect.y = pos.y;
  if (SDL_QueryTexture(texture.get(), nullptr, nullptr, &dstRect.w, &dstRect.h) == -1) {
    throw std::runtime_error(std::string("Failed to render text: ") + SDL_GetError());
  }
  dstRect.w *= 4;
  dstRect.h *= 4;
  
  SDL_RenderCopy(renderer, texture.get(), nullptr, &dstRect);
}

Pos RenderManager::textSize(const std::string &text) {
  static_assert(std::is_same<PosScalar, int>::value);
  Pos size;
  if (TTF_SizeText(font.get(), text.c_str(), &size.x, &size.y) == -1) {
    throw std::runtime_error(std::string("Failed to get text size: ") + TTF_GetError());
  }
  return size;
}

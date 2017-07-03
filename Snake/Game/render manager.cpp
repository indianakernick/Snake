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

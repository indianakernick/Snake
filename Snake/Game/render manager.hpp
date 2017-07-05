//
//  render manager.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_render_manager_hpp
#define game_render_manager_hpp

#include "types.hpp"
#include <SDL2/SDL_ttf.h>
#include <Unpacker/unpacker.hpp>
#include <Simpleton/Platform/renderer.hpp>
#include <Simpleton/Platform/font library.hpp>

class RenderManager {
public:
  RenderManager(Platform::Renderer &, Platform::FontLibrary &);
  
  void reset();
  
  void update(uint64_t);
  void renderBack();
  void renderTile(const std::string &, Pos, double = 0.0);
  void renderText(const std::string &, Color, Pos);
  Pos textSize(const std::string &);
  
private:
  Spritesheet sheet;
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
  std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)> font;
  SDL_Renderer *renderer;
  uint64_t animProg = 0;
};

#endif

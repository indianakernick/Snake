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
#include <unordered_map>
#include <SDL2/SDL_ttf.h>
#include <Unpacker/unpacker.hpp>
#include <Simpleton/SDL/renderer.hpp>
#include <Simpleton/SDL/font library.hpp>

class RenderManager {
public:
  RenderManager();
  
  void init(SDL::Renderer &);
  void quit();
  
  void reset();
  
  void update(uint64_t);
  void renderBack();
  void renderTile(const std::string &, Pos, double = 0.0);
  void renderTileReverse(const std::string &, Pos, double = 0.0);
  void renderText(int, const std::string &, Color, Pos);
  Pos textSize(int, const std::string &);
  
private:
  Unpack::Spritesheet sheet;
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
  std::unordered_map<int, std::unique_ptr<TTF_Font, decltype(&TTF_CloseFont)>> fonts;
  SDL_Renderer *renderer;
  uint64_t animProg = 0;
  
  void renderTileImpl(const std::string &, Pos, double);
  TTF_Font *getFont(int);
};

#endif

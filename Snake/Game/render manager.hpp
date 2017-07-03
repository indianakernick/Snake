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
#include <Unpacker/unpacker.hpp>
#include <Simpleton/Platform/renderer.hpp>

class RenderManager {
public:
  RenderManager(Platform::Renderer &, const std::string &);
  
  void reset();
  
  void update(uint64_t);
  void render(const std::string &, Pos, double = 0.0);
  
private:
  Spritesheet sheet;
  std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> texture;
  SDL_Renderer *renderer;
  uint64_t animProg = 0;
};

#endif

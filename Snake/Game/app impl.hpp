//
//  app impl.hpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_app_impl_hpp
#define game_app_impl_hpp

#include "rat.hpp"
#include "snake.hpp"
#include "sdl app.hpp"
#include "render manager.hpp"

class AppImpl final : public SDLApp {
public:
  AppImpl();
  
private:
  RenderManager renderMan;
  uint64_t animProg = 0;
  Snake snake;
  Rat rat;

  void init() override;
  bool input(uint64_t) override;
  bool update(uint64_t) override;
  void render(uint64_t) override;
  
  Pos getFoodPos() const;
  void snakeInput(SDL_Scancode);
};

#endif

//
//  app impl.hpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_app_impl_hpp
#define game_app_impl_hpp

#include <list>
#include "rat.hpp"
#include "snake.hpp"
#include "score.hpp"
#include "sdl app.hpp"
#include "power up.hpp"
#include "render manager.hpp"

class AppImpl final : public SDLApp {
public:
  AppImpl();
  
private:
  RenderManager renderMan;
  uint64_t animProg = 0;
  Snake snake;
  Rat rat;
  Score score;
  std::list<std::unique_ptr<PowerUp>> powerups;

  bool input(uint64_t) override;
  bool update(uint64_t) override;
  void render(uint64_t) override;
  
  bool shouldSpawnPowerUp() const;
  Pos getFreePos() const;
  void snakeInput(SDL_Scancode);
};

#endif

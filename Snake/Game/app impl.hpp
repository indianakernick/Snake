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
#include <vector>
#include "rat.hpp"
#include "snake.hpp"
#include "score.hpp"
#include "sdl app.hpp"
#include "item.hpp"
#include "render manager.hpp"

using ItemFactory = std::unique_ptr<Item> (*)(Pos);

class AppImpl final : public SDLApp {
public:
  AppImpl();
  
private:
  RenderManager renderMan;
  uint64_t animProg = 0;
  Snake snake;
  Rat rat;
  Score score;
  std::list<std::unique_ptr<Item>> items;
  std::vector<ItemFactory> itemFactories;
  std::vector<double> itemProbs;

  bool input(uint64_t) override;
  bool update(uint64_t) override;
  void render(uint64_t) override;
  
  void spawnItemIfShould();
  Pos getFreePos() const;
  void snakeInput(SDL_Scancode);
};

#endif

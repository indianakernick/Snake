//
//  snake.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_snake_hpp
#define game_snake_hpp

#include <deque>
#include "rat.hpp"
#include "render manager.hpp"

class Snake {
public:
  explicit Snake(Pos);
  
  void move(Math::Dir);
  bool isEating(const Rat &) const;
  bool colliding(Pos, bool = true) const;
  void tryToEat(Rat &);
  bool isDead() const;
  
  Pos head() const;
  Pos tail() const;
  
  void update();
  void render(RenderManager &) const;
  
private:
  std::deque<Pos> positions;
  Math::Dir currentDir = Math::Dir::LEFT;
  Math::Dir nextDir = Math::Dir::LEFT;
  bool eating = false;
  bool dead = false;
};

#endif

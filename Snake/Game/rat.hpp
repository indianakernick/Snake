//
//  rat.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_rat_hpp
#define game_rat_hpp

#include "render manager.hpp"

class Rat {
public:
  explicit Rat(Pos);
  
  void reset(Pos);
  Pos getPos() const;
  void eat();
  
  void render(RenderManager &) const;
  
private:
  Pos pos;
  bool beingEaten = false;
};

#endif

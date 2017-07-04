//
//  rat.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_rat_hpp
#define game_rat_hpp

#include "item.hpp"

class RenderManager;

class Rat : public Item {
public:
  explicit Rat(Pos);
  
  void render(RenderManager &) const override;
};

#endif

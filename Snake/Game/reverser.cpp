//
//  reverser.cpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "reverser.hpp"

Reverser::Reverser(const Pos pos)
  : ItemConfig(pos, LIFE_TIME) {}

SET_ITEM_SPRITE(Reverser, SPAWNING,       "reverser spawn");
SET_ITEM_SPRITE(Reverser, ALIVE,          "reverser");
SET_ITEM_SPRITE(Reverser, BEING_CONSUMED, "reverser");

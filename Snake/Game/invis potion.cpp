//
//  invis potion.cpp
//  Snake
//
//  Created by Indi Kernick on 7/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "invis potion.hpp"

InvisPotion::InvisPotion(const Pos pos)
  : ItemConfig(pos, LIFE_TIME) {}

SET_ITEM_SPRITE(InvisPotion, SPAWNING,       "invis potion spawn");
SET_ITEM_SPRITE(InvisPotion, ALIVE,          "invis potion");
SET_ITEM_SPRITE(InvisPotion, BEING_CONSUMED, "invis potion");

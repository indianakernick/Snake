//
//  coin.cpp
//  Snake
//
//  Created by Indi Kernick on 5/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "coin.hpp"

Coin::Coin(const Pos pos)
  : ItemConfig(pos) {}

SET_ITEM_SPRITE(Coin, SPAWNING,       "coin spawn");
SET_ITEM_SPRITE(Coin, ALIVE,          "coin");
SET_ITEM_SPRITE(Coin, BEING_CONSUMED, "coin");
SET_ITEM_SPRITE(Coin, DESPAWNING,     "coin");

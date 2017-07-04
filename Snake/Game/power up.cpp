//
//  power up.cpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "power up.hpp"

PowerUp::PowerUp(const Pos pos)
  : pos(pos) {}

Pos PowerUp::getPos() const {
  return pos;
}

bool PowerUp::isBeingConsumed() const {
  return beingConsumed;
}

void PowerUp::consume() {
  assert(not beingConsumed);
  beingConsumed = true;
}

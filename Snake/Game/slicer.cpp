//
//  slicer.cpp
//  Snake
//
//  Created by Indi Kernick on 5/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "slicer.hpp"

Slicer::Slicer(const Pos pos)
  : ItemConfig(pos) {}

SET_ITEM_SPRITE(Slicer, SPAWNING,       "slicer spawn");
SET_ITEM_SPRITE(Slicer, ALIVE,          "slicer");
SET_ITEM_SPRITE(Slicer, BEING_CONSUMED, "slicer");
SET_ITEM_SPRITE(Slicer, DESPAWNING,     "slicer");

//
//  coin.cpp
//  Snake
//
//  Created by Indi Kernick on 5/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "coin.hpp"

#include "render manager.hpp"

Coin::Coin(const Pos pos)
  : Item(pos) {}

void Coin::render(RenderManager &renderer) const {
  const State state = getState();
  if (state == State::SPAWNING) {
    renderer.renderTile("coin spawn", pos);
  } else {
    renderer.renderTile("coin", pos);
  }
}

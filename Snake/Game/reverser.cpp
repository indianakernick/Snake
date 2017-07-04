//
//  reverser.cpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "reverser.hpp"

#include "render manager.hpp"

Reverser::Reverser(const Pos pos)
  : Item(pos) {}

void Reverser::render(RenderManager &renderer) const {
  const State state = getState();
  if (state == State::SPAWNING) {
    renderer.renderTile("reverser spawn", pos);
  } else {
    renderer.renderTile("reverser", pos);
  }
}

std::unique_ptr<Item> makeReverser(const Pos pos) {
  return std::make_unique<Reverser>(pos);
}

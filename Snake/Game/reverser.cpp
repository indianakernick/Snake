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
  : PowerUp(pos) {}

void Reverser::render(RenderManager &renderer) const {
  renderer.renderTile("reverser", pos);
}

std::unique_ptr<PowerUp> makeReverser(const Pos pos) {
  return std::make_unique<Reverser>(pos);
}

//
//  slicer.cpp
//  Snake
//
//  Created by Indi Kernick on 5/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "slicer.hpp"

#include "render manager.hpp"

Slicer::Slicer(const Pos pos)
  : Item(pos) {}

void Slicer::render(RenderManager &renderer) const {
  const State state = getState();
  if (state == State::SPAWNING) {
    renderer.renderTile("slicer spawn", pos);
  } else {
    renderer.renderTile("slicer", pos);
  }
}

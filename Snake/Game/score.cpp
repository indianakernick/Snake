//
//  score.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "score.hpp"

#include "constants.hpp"
#include "render manager.hpp"

void Score::incr(const unsigned incrNum) {
  val += incrNum;
}

void Score::reset() {
  val = 0;
}

void Score::render(RenderManager &renderMan) const {
  renderMan.renderText("Score: " + std::to_string(val), SCORE_COLOR, {0, 0});
}

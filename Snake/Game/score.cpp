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
#include <Simpleton/Event/manager.hpp>

Score::Score() {
  evtMan->addListener(Utils::memFunWrap(this, &Score::onCoinConsumed));
}

void Score::incr(const unsigned incrNum) {
  val += incrNum;
}

void Score::reset() {
  val = 0;
}

void Score::render(RenderManager &renderMan) const {
  renderMan.renderText(SCORE_FONT_SIZE, "Score: " + std::to_string(val), TEXT_COLOR, {0, 0});
}

void Score::onCoinConsumed(const CoinConsumed::Ptr) {
  val += CoinConsumed::SCORE;
}

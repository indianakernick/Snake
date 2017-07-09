//
//  score.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "score.hpp"

#include <fstream>
#include "constants.hpp"
#include "render manager.hpp"
#include <Simpleton/Platform/system info.hpp>
#include <Simpleton/Event/manager.hpp>

std::string highscorePath() {
  return Platform::getSaveDir("Indi Kernick", "Snake") + "highscore.txt";
}

void Score::init() {
  evtMan->addListener(Utils::memFunWrap(this, &Score::onCoinConsumed));
  std::ifstream file(highscorePath());
  if (file.is_open() && file.peek() != EOF) {
    file >> high;
  }
}

void Score::quit() {
  std::ofstream file(highscorePath());
  if (file.is_open()) {
    file << high;
  }
}

void Score::incr(const unsigned incrNum) {
  val += incrNum;
  if (val > high) {
    high = val;
  }
}

void Score::reset() {
  val = 0;
}

void Score::render(RenderManager &renderMan) const {
  renderMan.renderText(SCORE_FONT_SIZE, "Score: " + std::to_string(val), TEXT_COLOR, {0, 0});
  const int height = renderMan.textSize(SCORE_FONT_SIZE, "S").y;
  renderMan.renderText(SCORE_FONT_SIZE, "Highscore: " + std::to_string(high), TEXT_COLOR, {0, height});
}

void Score::onCoinConsumed(const CoinConsumed::Ptr) {
  incr(CoinConsumed::SCORE);
}

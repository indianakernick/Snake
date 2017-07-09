//
//  score.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_score_hpp
#define game_score_hpp

#include "coin consumed.hpp"

class RenderManager;

class Score {
public:
  Score() = default;
  
  void init();
  void quit();
  
  void incr(unsigned = 1);
  void reset();
  
  void render(RenderManager &) const;
  
private:
  unsigned val = 0;
  unsigned high = 0;
  
  void onCoinConsumed(CoinConsumed::Ptr);
};

#endif

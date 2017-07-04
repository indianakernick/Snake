//
//  score.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_score_hpp
#define game_score_hpp

class RenderManager;

class Score {
public:
  Score() = default;
  
  void incr(unsigned = 1);
  void reset();
  
  void render(RenderManager &) const;
  
private:
  unsigned val = 0;
};

#endif

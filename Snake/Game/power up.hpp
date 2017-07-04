//
//  power up.hpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_power_up_hpp
#define game_power_up_hpp

#include "types.hpp"

class RenderManager;

class PowerUp {
public:
  explicit PowerUp(Pos);
  virtual ~PowerUp() = default;
  
  Pos getPos() const;
  bool isBeingConsumed() const;
  virtual void consume();
  
  virtual void render(RenderManager &) const = 0;

protected:
  Pos pos;

private:
  bool beingConsumed = false;
};

#endif

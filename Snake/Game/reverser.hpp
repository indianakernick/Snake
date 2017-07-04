//
//  reverser.hpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_reverser_hpp
#define game_reverser_hpp

#include <memory>
#include "power up.hpp"

class Reverser : public PowerUp {
public:
  Reverser(Pos);
  
  void render(RenderManager &) const override;
};

std::unique_ptr<PowerUp> makeReverser(Pos);

#endif

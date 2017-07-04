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
#include "item.hpp"

class Reverser : public Item {
public:
  Reverser(Pos);
  
  void render(RenderManager &) const override;
};

std::unique_ptr<Item> makeReverser(Pos);

#endif

//
//  slicer.hpp
//  Snake
//
//  Created by Indi Kernick on 5/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_slicer_hpp
#define game_slicer_hpp

#include "item.hpp"

class Slicer : public Item {
public:
  static constexpr Rarity RARITY = Rarity::HIGH;

  Slicer(Pos);
  
  void render(RenderManager &) const override;
};

#endif

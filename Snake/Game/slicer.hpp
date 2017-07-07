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

class Slicer : public ItemConfig<Slicer> {
public:
  static constexpr Rarity RARITY = Rarity::HIGH;
  static constexpr unsigned LIFE_TIME = 50;

  Slicer(Pos);
};

#endif

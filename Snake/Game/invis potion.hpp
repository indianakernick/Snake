//
//  invis potion.hpp
//  Snake
//
//  Created by Indi Kernick on 7/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_invis_potion_hpp
#define game_invis_potion_hpp

#include "item.hpp"

class InvisPotion : public Powerup {
public:
  static constexpr Rarity RARITY = Rarity::HIGH;
  static constexpr unsigned LIFE_TIME = 50;
  
  explicit InvisPotion(Pos);
};

#endif

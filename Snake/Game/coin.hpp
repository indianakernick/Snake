//
//  coin.hpp
//  Snake
//
//  Created by Indi Kernick on 5/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_coin_hpp
#define game_coin_hpp

#include "item.hpp"

class Coin : public Powerup {
public:
  static constexpr Rarity RARITY = Rarity::VERY_HIGH;
  static constexpr unsigned LIFE_TIME = 40;
  
  explicit Coin(Pos);
};

#endif

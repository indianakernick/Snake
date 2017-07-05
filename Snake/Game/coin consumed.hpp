//
//  coin consumed.hpp
//  Snake
//
//  Created by Indi Kernick on 6/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_coin_consumed_hpp
#define game_coin_consumed_hpp

#include <Simpleton/Event/event.hpp>

constexpr char COIN_CONSUMED_NAME[] = "Coin Consumed";

class CoinConsumed : public Game::StaticEvent<CoinConsumed, COIN_CONSUMED_NAME> {
public:
  static constexpr unsigned SCORE = 100;

  CoinConsumed() = default;
};

#endif

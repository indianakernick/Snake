//
//  item.hpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_item_hpp
#define game_item_hpp

#include <memory>
#include "types.hpp"
#include "render manager.hpp"

class Item {
public:
  enum class Rarity {
    LOW,
    MEDIUM,
    HIGH,
    VERY_HIGH
  };

  explicit Item(Pos, unsigned = std::numeric_limits<unsigned>::max());
  virtual ~Item() = default;
  
  Pos getPos() const;
  bool isAlive() const;
  bool isDespawning() const;
  void consume();
  
  virtual void update();
  virtual void render(RenderManager &) const = 0;

protected:
  Pos pos;

  enum class State : uint8_t {
    SPAWNING,
    ALIVE,
    BEING_CONSUMED,
    DESPAWNING
  };
  
  State getState() const;

private:
  unsigned timeTillDeath;
  State state = State::SPAWNING;
};

template <typename ItemType>
std::enable_if_t<
  std::is_base_of<Item, ItemType>::value &&
  !std::is_same<Item, ItemType>::value,
  std::unique_ptr<Item>
>
makeItem(const Pos pos) {
  return std::make_unique<ItemType>(pos);
}

class Powerup : public Item {
public:
  explicit Powerup(Pos);
  Powerup(Pos, unsigned);
  ~Powerup() = default;
  
  void render(RenderManager &) const override;
};

#endif

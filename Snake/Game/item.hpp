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

template <typename Subclass>
class ItemConfig : public Item {
public:
  ItemConfig(const Pos pos)
    : Item(pos) {}
  ItemConfig(const Pos pos, const unsigned lifeTime)
    : Item(pos, lifeTime) {}
  ~ItemConfig() {
    static_assert(std::is_base_of<ItemConfig, Subclass>::value);
  }
  
  void render(RenderManager &renderMan) const override {
    #define CASE(STATE)                                                         \
      case State::STATE:                                                        \
        if (STATE##_SPRITE) {                                                   \
          renderMan.renderTile(STATE##_SPRITE, pos);                            \
        }                                                                       \
        break
    
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wundefined-var-template"
    
    switch (getState()) {
      CASE(SPAWNING);
      CASE(ALIVE);
      CASE(BEING_CONSUMED);
      case State::DESPAWNING:
        if (SPAWNING_SPRITE) {
          renderMan.renderTileReverse(SPAWNING_SPRITE, pos);
        }
    }
    
    #pragma clang diagnostic pop
    
    #undef CASE
  }
  
protected:
  static const char *const SPAWNING_SPRITE;
  static const char *const ALIVE_SPRITE;
  static const char *const BEING_CONSUMED_SPRITE;
};

#define SET_ITEM_SPRITE(SUB_CLASS, STATE, PATH) template <> \
  const char *const ItemConfig<SUB_CLASS>::STATE##_SPRITE = PATH

#endif

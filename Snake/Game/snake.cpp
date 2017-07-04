//
//  snake.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "snake.hpp"

#include "rat.hpp"
#include "power up.hpp"
#include "reverser.hpp"
#include "constants.hpp"
#include "render manager.hpp"

namespace {
  using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
  
  //returns a vector pointing from this to prev
  Pos getDirVec(const Pos thisPos, const Pos prevPos) {
    Pos prevToThis = thisPos - prevPos;
    
    //this accounts for the snake going into one side of the window and
    //and coming out from the opposite side of the window
    if (prevToThis.x > 1) {
      prevToThis.x -= GAME_SIZE.x;
    }
    if (prevToThis.y > 1) {
      prevToThis.y -= GAME_SIZE.y;
    }
    if (prevToThis.x < -1) {
      prevToThis.x += GAME_SIZE.x;
    }
    if (prevToThis.y < -1) {
      prevToThis.y += GAME_SIZE.y;
    }
    
    return prevToThis;
  }

}

Snake::Snake(const Pos center)
  : positions({
      {center.x - 1, center.y},
      center,
      {center.x + 1, center.y},
      {center.x + 2, center.y}
    }) {}

void Snake::move(const Math::Dir newDir) {
  if (Math::opposite(newDir) != currentDir) {
    nextDir = newDir;
  }
}

bool Snake::isEating(const Rat &rat) const {
  return eating && positions.front() == rat.getPos();
}

bool Snake::colliding(const Pos pos, const bool withHead) const {
  return std::any_of(
    positions.cbegin() + !withHead,
    positions.cend(),
    [pos] (const Pos thisPos) {
      return pos == thisPos;
    }
  );
}

void Snake::tryToEat(Rat &rat) {
  if (head() == rat.getPos()) {
    eating = true;
    rat.eat();
  }
}

bool Snake::isDead() const {
  return dead;
}

void Snake::tryToConsume(PowerUp &powerup) {
  if (head() == powerup.getPos() && not powerup.isBeingConsumed()) {
    powerup.consume();
  }
}

bool Snake::finishConsuming(PowerUp &powerup) {
  if (positions[1] == powerup.getPos() && powerup.isBeingConsumed()) {
    PowerUp *const ptr = &powerup;
    if (PowerUp *reverser = dynamic_cast<Reverser *>(ptr)) {
      std::reverse(positions.begin(), --positions.end());
      currentDir = FromVec::conv(getDirVec(positions[0], positions[1]));
      nextDir = currentDir;
    }
    return true;
  }
  return false;
}

Pos Snake::head() const {
  return positions.front();
}

Pos Snake::tail() const {
  return positions.back();
}

void Snake::update() {
  if (eating) {
    eating = false;
  } else {
    positions.pop_back();
  }
  positions.push_front((positions.front() + ToVec::conv(nextDir) + GAME_SIZE) % GAME_SIZE);
  currentDir = nextDir;
  if (colliding(head(), false) && head() != tail()) {
    dead = true;
  }
}

namespace {
  double getBodySpriteAngle(const Math::Dir prevToThisDir) {
    return Math::ToNum<double>::conv(prevToThisDir, 90.0);
  }

  std::string getBodySpriteTurnName(const Math::Dir prevToThisDir, const Math::Dir thisToNextDir) {
    const Math::SignedDirType diff = Math::diff(prevToThisDir, thisToNextDir);
    if (diff == 0) {
      return "straight";
    } else if (diff == 1) {
      return "right corner";
    } else if (diff == -1) {
      return "left corner";
    } else {
      assert(false);
      return "";
    }
  }

  void renderBody(
    RenderManager &renderer,
    const std::string &name,
    const Pos pos,
    const Pos prevToThisVec,
    const Pos thisToNextVec
  ) {
    using FromVec = Math::FromVec<double, Math::Dir::RIGHT, Math::Dir::DOWN>;
    
    const Math::Dir prevToThisDir = FromVec::conv(prevToThisVec);
    const Math::Dir thisToNextDir = FromVec::conv(thisToNextVec);
    renderer.renderTile(
      name + ' ' + getBodySpriteTurnName(prevToThisDir, thisToNextDir),
      pos,
      getBodySpriteAngle(prevToThisDir)
    );
  }

  std::string dirToString(const Math::Dir dir) {
    static const std::string NAMES[4] = {
      "up", "right", "down", "left"
    };
    return NAMES[Math::ToNum<size_t>::conv(dir)];
  }
}

void Snake::render(RenderManager &renderer) const {
  if (eating) {
    renderer.renderTile("eat rat " + dirToString(currentDir), head());
  } else {
    renderer.renderTile("head", head(), getBodySpriteAngle(currentDir));
  }

  Pos nextPos = positions[2];
  Pos thisToNextVec = getDirVec(positions[1], nextPos);
  renderBody(renderer, "head", positions[1], thisToNextVec, getDirVec(positions[0], positions[1]));
  
  for (auto b = positions.cbegin() + 2; b != positions.cend() - 2; ++b) {
    const Pos pos = nextPos;
    nextPos = *(b + 1);
    const Pos prevToThisVec = getDirVec(pos, nextPos);
    renderBody(renderer, "body", pos, prevToThisVec, thisToNextVec);
    thisToNextVec = prevToThisVec;
  }
  
  const Pos prevToThisVec = getDirVec(nextPos, positions.back());
  renderBody(renderer, eating ? "body" : "tail", nextPos, prevToThisVec, thisToNextVec);
  thisToNextVec = prevToThisVec;

  const double tailAngle = getBodySpriteAngle(FromVec::conv(thisToNextVec));
  renderer.renderTile(eating ? "tail grow" : "tail", tail(), tailAngle);
}

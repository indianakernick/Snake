//
//  app.hpp
//  Snake
//
//  Created by Indi Kernick on 21/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_app_hpp
#define game_app_hpp

#include <Simpleton/Time/delta.hpp>

class App {
public:
  App() = default;
  virtual ~App() = default;
  
  void mainLoop();
  
private:
  virtual void init() {}
  virtual bool input(uint64_t) = 0;
  virtual bool update(uint64_t) = 0;
  virtual void render(uint64_t) = 0;
  virtual void quit() {}
};

#endif

//
//  sdl app.hpp
//  Snake
//
//  Created by Indi Kernick on 21/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_sdl_app_hpp
#define game_sdl_app_hpp

#include "app.hpp"
#include <Simpleton/Platform/window library.hpp>

class SDLApp : public App {
public:
  SDLApp(const Platform::Window::Desc &, bool);

  Platform::WindowLibrary lib;
  Platform::Window window;
  Platform::Renderer renderer;
};

#endif

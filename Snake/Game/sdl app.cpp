//
//  sdl app.cpp
//  Snake
//
//  Created by Indi Kernick on 21/6/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "sdl app.hpp"

SDLApp::SDLApp(const Platform::Window::Desc &windowDesc, const bool vsync)
  : lib(),
    fontLib(),
    window(lib.makeWindow(windowDesc)),
    renderer(lib.makeRenderer(window, vsync)) {}

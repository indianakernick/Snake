//
//  types.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_types_hpp
#define game_types_hpp

#include <glm/vec4.hpp>
#include <Simpleton/Grid/dir.hpp>

using Color = glm::tvec4<uint8_t>;
using PosScalar = int;
using Pos = glm::tvec2<PosScalar>;
constexpr Grid::ToVec<PosScalar, Grid::Dir::right, Grid::Dir::down> toVec;

#endif

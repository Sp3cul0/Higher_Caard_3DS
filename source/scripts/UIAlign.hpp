#pragma once

struct Vector2 {
    float x, y;
};

namespace UIAlign {
    const Vector2 TOP_LEFT      = {0,   0};
    const Vector2 TOP_CENTER    = {200, 0};
    const Vector2 TOP_RIGHT     = {400, 0};
    const Vector2 CENTER_LEFT   = {0,   120};
    const Vector2 CENTER        = {142, 105};
    const Vector2 CENTER_RIGHT  = {400, 100};
    const Vector2 BOTTOM_LEFT   = {0,   240};
    const Vector2 BOTTOM_CENTER = {200, 240};
    const Vector2 BOTTOM_RIGHT  = {400, 240};
}

namespace UIAlignBot {
    const Vector2 TOP_LEFT      = {0,   0};
    const Vector2 TOP_CENTER    = {160, 0};
    const Vector2 TOP_RIGHT     = {320, 0};
    const Vector2 CENTER        = {160, 120};
    const Vector2 BOTTOM_LEFT   = {0,   240};
    const Vector2 BOTTOM_CENTER = {160, 240};
    const Vector2 BOTTOM_RIGHT  = {320, 240};
}
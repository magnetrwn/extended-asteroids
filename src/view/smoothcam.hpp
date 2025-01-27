#ifndef SMOOTHCAM_HPP_
#define SMOOTHCAM_HPP_

#include "typedef.hpp"
#include "util.hpp"

class SmoothCamera {
private:
    const f32 OK_DIST;
    const f32 MAX_SPEED;
    const f32 DIST_SCALE;
    f32_2 current_pos;
    f32_2 target_pos;

public:
    SmoothCamera(f32_2 pos) : OK_DIST(0.05f), MAX_SPEED(0.3f), DIST_SCALE(0.06f), current_pos(pos), target_pos(pos) {}

    inline f32_2 get() const { return current_pos; }
    inline void set(f32_2 pos) { current_pos = pos; target_pos = pos; }
    inline void target(f32_2 pos) { target_pos = pos; }

    inline void step(f32 scale) { 
        const f32_2 diff = { target_pos.x - current_pos.x, target_pos.y - current_pos.y };
        f32 dist2 = diff.x * diff.x + diff.y * diff.y;

        if (dist2 > OK_DIST) {
            util::clamp_h(dist2, MAX_SPEED);
            current_pos.x += diff.x * dist2 * DIST_SCALE * scale;
            current_pos.y += diff.y * dist2 * DIST_SCALE * scale;
        }
    }
};

#endif

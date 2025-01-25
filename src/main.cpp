#include <raylib.h>
#include "asteroid.hpp"
#include "util.hpp"

int main() {
    // [Settings.Window]
    const f32 WINDOW_W = util::cfg_f32("Settings.Window", "WINDOW_W");
    const f32 WINDOW_H = util::cfg_f32("Settings.Window", "WINDOW_H");
    const usize WINDOW_FPS = util::cfg_usize("Settings.Window", "WINDOW_FPS");
    const bool WINDOW_VSYNC = util::cfg_bool("Settings.Window", "WINDOW_VSYNC");

    SetTargetFPS(WINDOW_FPS);
    if (WINDOW_VSYNC)
        SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_W, WINDOW_H, "Extended Asteroids");

    Asteroid astarr[25];
    for (isize i = 0; i < 5; ++i)
        for (isize j = 0; j < 5; ++j) {
            f32 off_x = (j - 2) * WINDOW_W / 5;
            f32 off_y = (i - 2) * WINDOW_H / 5;

            astarr[i * 5 + j] = Asteroid({ WINDOW_W / 2 + off_x, WINDOW_H / 2 + off_y });
        }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });
        DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 72, RAYWHITE);

        for (usize i = 0; i < 25; ++i)
            DrawLineStrip(const_cast<Vector2*>(astarr[i].get_shape_vtx_array()), astarr[i].get_shape_vtx_count(), RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
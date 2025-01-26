#include <raylib.h>
#include "asteroid.hpp"
#include "util.hpp"

int main() {
    static constexpr f32 ANIM_BASE_GAME_FPS = 60.0f;

    // [Settings.Window]
    const f32 WINDOW_W = util::cfg_f32("Settings.Window", "WINDOW_W");
    const f32 WINDOW_H = util::cfg_f32("Settings.Window", "WINDOW_H");
    const usize WINDOW_FPS = util::cfg_usize("Settings.Window", "WINDOW_FPS");
    const bool WINDOW_VSYNC = util::cfg_bool("Settings.Window", "WINDOW_VSYNC");

    SetTargetFPS(WINDOW_FPS);
    if (WINDOW_VSYNC)
        SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_W, WINDOW_H, "Extended Asteroids");

    //InitAudioDevice();
    //Sound theme_bgm = LoadSound(util::cfg_string("Resources.Audio", "THEME_BGM_PATH").c_str());

    Asteroid astarr[25];
    for (isize i = 0; i < 5; ++i)
        for (isize j = 0; j < 5; ++j) {
            f32 off_x = (j - 2) * WINDOW_W / 5;
            f32 off_y = (i - 2) * WINDOW_H / 5;

            astarr[i * 5 + j].set_position({ WINDOW_W / 2 + off_x, WINDOW_H / 2 + off_y });
            astarr[i * 5 + j].set_angular_velocity(util::randf() * 0.1f - 0.05f);
            astarr[i * 5 + j].set_velocity({ util::randf() * 2.0f - 1.0f, util::randf() * 2.0f - 1.0f });
        }
    
    bool already_drawn[25];

    //PlaySound(theme_bgm);

    while (!WindowShouldClose()) {
        //if (!IsSoundPlaying(theme_bgm))
        //    PlaySound(theme_bgm);

        float dt_scale = GetFrameTime() * ANIM_BASE_GAME_FPS;

        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });
        DrawText(std::to_string(GetFPS()).c_str(), 10, 6, 40, WHITE);

        for (usize i = 0; i < 25; ++i)
            already_drawn[i] = false;

        for (usize i = 0; i < 24; ++i)
            for (usize j = i + 1; j < 25; ++j)
                if (astarr[i].is_collision(astarr[j])) {
                    DrawLineStrip(const_cast<Vector2*>(astarr[i].get_entity_vtx_array()), astarr[i].get_entity_vtx_count(), RED);
                    DrawLineStrip(const_cast<Vector2*>(astarr[j].get_entity_vtx_array()), astarr[j].get_entity_vtx_count(), RED);
                    already_drawn[i] = true;
                    already_drawn[j] = true;
                }

        for (usize i = 0; i < 25; ++i)
            if (!already_drawn[i])
                DrawLineStrip(const_cast<Vector2*>(astarr[i].get_entity_vtx_array()), astarr[i].get_entity_vtx_count(), WHITE);

        for (usize i = 0; i < 25; ++i) {
            astarr[i].step(dt_scale);

            if (astarr[i].get_position().x < 0.0f or astarr[i].get_position().x > WINDOW_W)
                astarr[i].set_position({ astarr[i].get_position().x < 0.0f ? WINDOW_W : 0.0f, astarr[i].get_position().y });
            if (astarr[i].get_position().y < 0.0f or astarr[i].get_position().y > WINDOW_H)
                astarr[i].set_position({ astarr[i].get_position().x, astarr[i].get_position().y < 0.0f ? WINDOW_H : 0.0f });
        }

        EndDrawing();
    }

    //UnloadSound(theme_bgm);
    //CloseAudioDevice();
    CloseWindow();
    return 0;
}
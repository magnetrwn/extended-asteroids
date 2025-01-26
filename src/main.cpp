#include <raylib.h>
#include "asteroid.hpp"
#include "util.hpp"
#include "rover.hpp"
#include "world.hpp"

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

    World world({ 0.0f, 0.0f }, { WINDOW_W, WINDOW_H });
    for (usize i = 0; i < 60; ++i)
        world.spawn_asteroid_nearby({ WINDOW_W / 2, WINDOW_H / 2 }, 2400.0f);

    Rover rover;
    rover.set_position({ WINDOW_W / 2, WINDOW_H / 2 });

    //PlaySound(theme_bgm);

    while (!WindowShouldClose()) {
        //if (!IsSoundPlaying(theme_bgm))
        //    PlaySound(theme_bgm);

        const f32_2 world_pos = world.get_position();
        const float dt_scale = GetFrameTime() * ANIM_BASE_GAME_FPS;

        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });
        DrawText(std::to_string(GetFPS()).c_str(), 10, 6, 40, WHITE);

        for (usize i = 0; i < world.get_asteroid_count(); ++i) {
            Vector2* vertexes = const_cast<Vector2*>(world.get_asteroid(i).get_entity_vtx_array());
            const usize vtx_count = world.get_asteroid(i).get_entity_vtx_count();

            util::offsetv(vertexes, vtx_count, { -world_pos.x, -world_pos.y });
            DrawLineStrip(vertexes, vtx_count, WHITE);
        }

        Vector2* vertexes = const_cast<Vector2*>(rover.get_entity_vtx_array());
        const usize vtx_count = rover.get_entity_vtx_count();
        util::offsetv(vertexes, vtx_count, { -world_pos.x, -world_pos.y });
        DrawLineStrip(vertexes, vtx_count, GREEN);

        EndDrawing();

        world.step(dt_scale);
        world.add_position({ 0.0f, -0.1f });
    }

    //UnloadSound(theme_bgm);
    //CloseAudioDevice();
    //CloseWindow();
    return 0;
}
#include <raylib.h>
#include "asteroid.hpp"
#include "util.hpp"
#include "rover.hpp"
#include "world.hpp"

void draw(Entity* e, f32_2 offset, Color color) {
    Vector2* vtx = const_cast<Vector2*>(e->get_entity_vtx_array());
    util::offsetv(vtx, e->get_entity_vtx_count(), { -offset.x, -offset.y });
    DrawLineStrip(vtx, e->get_entity_vtx_count(), color);
}

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
    //for (usize i = 0; i < 60; ++i)
        //world.spawn_asteroid_nearby({ WINDOW_W / 2, WINDOW_H / 2 }, 2400.0f);

    Rover rover;
    rover.set_position({ WINDOW_W / 2, WINDOW_H / 2 });

    //PlaySound(theme_bgm);

    double next_asteroid_spawn = GetTime();

    while (!WindowShouldClose()) {
        //if (!IsSoundPlaying(theme_bgm))
        //    PlaySound(theme_bgm);

        const f32_2 world_pos = world.get_position();
        const float dt_scale = GetFrameTime() * ANIM_BASE_GAME_FPS;

        BeginDrawing();
        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });
        DrawText(std::to_string(GetFPS()).c_str(), 10, 6, 40, WHITE);

        for (usize i = 0; i < world.get_asteroid_count(); ++i)
            draw(&world.get_asteroid(i), world_pos, WHITE);

        draw(&rover, world_pos, GREEN);

        EndDrawing();

        world.step(dt_scale);
        world.add_position({ 0.0f, -7.0f * dt_scale });
    }

    //UnloadSound(theme_bgm);
    //CloseAudioDevice();
    //CloseWindow();
    return 0;
}
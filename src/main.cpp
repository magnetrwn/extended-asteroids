#include <raylib.h>
#include "asteroid.hpp"
#include "util.hpp"
#include "rover.hpp"
#include "world.hpp"
#include "smoothcam.hpp"

void draw(Entity* e, f32_2 offset, Color color) {
    Vector2* vtx = const_cast<Vector2*>(e->get_entity_vtx_array());
    util::offsetv(vtx, e->get_entity_vtx_count(), { -offset.x, -offset.y });
    DrawLineStrip(vtx, e->get_entity_vtx_count(), color);
}

void draw_fill(const f32_2* vertexes, usize vtx_count, f32_2 offset, Color color) {
    Vector2* vtx = const_cast<Vector2*>(vertexes);
    util::offsetv(vtx, vtx_count, { -offset.x, -offset.y });
    DrawTriangleStrip(vtx, vtx_count, color);
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
    world.get_rover().set_position({ WINDOW_W / 2, WINDOW_H / 2 });

    SmoothCamera cam({ 0.0f, 0.0f });

    //PlaySound(theme_bgm);

    double next_asteroid_spawn = GetTime();

    while (!WindowShouldClose()) {
        //if (!IsSoundPlaying(theme_bgm))
        //    PlaySound(theme_bgm);

        const float dt_scale = GetFrameTime() * ANIM_BASE_GAME_FPS;
        f32_2 rover_vel = world.get_rover().get_velocity();

        const u8 rover_up_vel_to_alpha = (rover_vel.y < 0.0f) ? static_cast<u8>(-rover_vel.y * 50.0f) : 0;
        const u8 rover_down_vel_to_alpha = (rover_vel.y > 0.0f) ? static_cast<u8>(rover_vel.y * 50.0f) : 0;
        const u8 rover_left_vel_to_alpha = (rover_vel.x < 0.0f) ? static_cast<u8>(-rover_vel.x * 50.0f) : 0;
        const u8 rover_right_vel_to_alpha = (rover_vel.x > 0.0f) ? static_cast<u8>(rover_vel.x * 50.0f) : 0;

        BeginDrawing();

        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });
        DrawText(std::to_string(GetFPS()).c_str(), 10, 6, 40, WHITE);
        for (usize i = 0; i < world.get_asteroid_count(); ++i)
            draw(&world.get_asteroid(i), world.get_position(), WHITE);
        draw(&world.get_rover(), world.get_position(), GREEN);
        //DrawCircle(rel_rover_pos.x, rel_rover_pos.y, 5.0f, RED); // for a future fuel mechanic, destroy asteroids to get circles for fuel/attacks
        draw_fill(world.get_rover().get_triangle_pair(Rover::UP), 6, cam.get(), Color{ 0x00, 0xff, 0x00, rover_up_vel_to_alpha });
        draw_fill(world.get_rover().get_triangle_pair(Rover::DOWN), 6, cam.get(), Color{ 0x00, 0xff, 0x00, rover_down_vel_to_alpha });
        draw_fill(world.get_rover().get_triangle_pair(Rover::LEFT), 6, cam.get(), Color{ 0x00, 0xff, 0x00, rover_left_vel_to_alpha });
        draw_fill(world.get_rover().get_triangle_pair(Rover::RIGHT), 6, cam.get(), Color{ 0x00, 0xff, 0x00, rover_right_vel_to_alpha });

        EndDrawing();

        if (IsKeyDown(KEY_W))
            world.get_rover().add_velocity_forward(0.21f * dt_scale);
        if (IsKeyDown(KEY_A))
            world.get_rover().add_angular_velocity(-0.003f * dt_scale);
        if (IsKeyDown(KEY_D))
            world.get_rover().add_angular_velocity(0.003f * dt_scale);

        if (!IsKeyDown(KEY_W))
            world.get_rover().dampen_velocity(0.98f); // TODO this is an issue for delta time scaling
        if (!IsKeyDown(KEY_A) and !IsKeyDown(KEY_D))
            world.get_rover().dampen_angular_velocity(0.98f);

        rover_vel = world.get_rover().get_velocity();
        f32 rover_angular_vel = world.get_rover().get_angular_velocity();
        util::clamp_lh(rover_vel.x, -9.0f, 9.0f);
        util::clamp_lh(rover_vel.y, -9.0f, 9.0f);
        util::clamp_lh(rover_angular_vel, -0.1f, 0.1f);
        world.get_rover().set_velocity(rover_vel);
        world.get_rover().set_angular_velocity(rover_angular_vel);

        cam.target({ world.get_rover().get_position().x - WINDOW_W / 2, world.get_rover().get_position().y - WINDOW_H / 2 });
        world.step(dt_scale);
        cam.step(dt_scale);
        world.set_position(cam.get());

        //world.add_position({ 0.0f, -7.0f * dt_scale });

        //if (GetTime() > next_asteroid_spawn + 0.5f) {
        //    world.spawn_asteroid_nearby(world_pos, 2400.0f);
        //    next_asteroid_spawn = GetTime();
        //}
    }

    //UnloadSound(theme_bgm);
    //CloseAudioDevice();
    //CloseWindow();
    return 0;
}
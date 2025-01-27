#include <raylib.h>
#include "asteroid.hpp"
#include "util.hpp"
#include "rover.hpp"
#include "world.hpp"
#include "smoothcam.hpp"
#include "ltmath.hpp"

using namespace LookupTableMath;

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
    static constexpr f32 MAX_ROVER_VEL = 9.0f;
    static constexpr f32 MIN_ROVER_VEL = -MAX_ROVER_VEL;

    // [Settings.Window]
    const f32 WINDOW_W = util::cfg_f32("Settings.Window", "WINDOW_W");
    const f32 WINDOW_H = util::cfg_f32("Settings.Window", "WINDOW_H");
    const usize WINDOW_FPS = util::cfg_usize("Settings.Window", "WINDOW_FPS");
    const bool WINDOW_VSYNC = util::cfg_bool("Settings.Window", "WINDOW_VSYNC");

    SetTargetFPS(WINDOW_FPS);
    if (WINDOW_VSYNC)
        SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(WINDOW_W, WINDOW_H, "Extended Asteroids");

    InitAudioDevice();
    Sound theme_bgm = LoadSound(util::cfg_string("Resources.Audio", "THEME_BGM_PATH").c_str());
    SetSoundVolume(theme_bgm, 0.5f);

    World world(
        { 0.0f, 0.0f }, 
        { WINDOW_W, WINDOW_H }, 
        [] { PlaySound(LoadSound(util::cfg_string("Resources.Audio", "MOONCOIN_SFX_PATH").c_str())); }, 
        [] { PlaySound(LoadSound(util::cfg_string("Resources.Audio", "COLLISION_SFX_PATH").c_str())); }
    );
    world.get_rover().set_position({ WINDOW_W / 2, WINDOW_H / 2 });

    SmoothCamera cam({ 0.0f, 0.0f });

    PlaySound(theme_bgm);

    double next_asteroid_spawn = GetTime();
    double next_mooncoin_spawn = GetTime();

    while (!WindowShouldClose()) {
        if (!IsSoundPlaying(theme_bgm))
            PlaySound(theme_bgm);

        const f32 dt_scale = GetFrameTime() * ANIM_BASE_GAME_FPS;
        
        f32_2 rover_vel = world.get_rover().get_velocity();

        const f32_2 centered_view_of_rover = { world.get_rover().get_position().x - WINDOW_W / 2, world.get_rover().get_position().y - WINDOW_H / 2 };

        const f32 rover_angle = world.get_rover().get_angle();
        const f32_2 rover_fill_pos = { 
            world.get_position().x - cam.get().x,
            world.get_position().y - cam.get().y
        };
        const u8 rover_alphas[4] = {
            static_cast<u8>((1 + ltcosf(rover_angle)) * 255.0f / 4.0f),
            static_cast<u8>((1 + ltcosf(rover_angle + M_PI)) * 255.0f / 4.0f),
            static_cast<u8>((1 + ltcosf(rover_angle + 3 * M_PI / 2)) * 255.0f / 4.0f),
            static_cast<u8>((1 + ltcosf(rover_angle + M_PI / 2)) * 255.0f / 4.0f)
        };

        BeginDrawing();

        ClearBackground(Color{ 0x27, 0x28, 0x22, 0xff });
        for (usize i = 0; i < world.get_mooncoin_count(); ++i)
            draw(&world.get_mooncoin(i), world.get_position(), Color{ 0x00, 0xff, 0x00, 0xff });

        for (usize i = 0; i < world.get_asteroid_count(); ++i)
            draw(&world.get_asteroid(i), world.get_position(), WHITE);

        draw(&world.get_rover(), world.get_position(), GREEN);

        //DrawCircle(rover_fill_pos.x, rover_fill_pos.y, 50.0f, RED); // TODO for a future fuel mechanic, destroy asteroids to get circles for fuel/attacks

        draw_fill(world.get_rover().get_triangle_pair(Rover::UP), 6, rover_fill_pos, Color{ 0x00, 0xff, 0x00, rover_alphas[0] });
        draw_fill(world.get_rover().get_triangle_pair(Rover::DOWN), 6, rover_fill_pos, Color{ 0x00, 0xff, 0x00, rover_alphas[1] });
        draw_fill(world.get_rover().get_triangle_pair(Rover::LEFT), 6, rover_fill_pos, Color{ 0x00, 0xff, 0x00, rover_alphas[2] });
        draw_fill(world.get_rover().get_triangle_pair(Rover::RIGHT), 6, rover_fill_pos, Color{ 0x00, 0xff, 0x00, rover_alphas[3] });

        /* UI */

        DrawRectangle(0, 0, WINDOW_W, 80, Color{ 0x20, 0x20, 0x20, 0xa0 });
        DrawText(std::to_string(GetFPS()).c_str(), 10, 6, 40, WHITE);

        DrawRectangle(0, WINDOW_H - 80, WINDOW_W, 80, Color{ 0x20, 0x20, 0x20, 0xa0 });

        DrawText("SPEED", 10, WINDOW_H - 46, 30, WHITE);
        DrawRectangle(9, WINDOW_H - 54, 400, 6, Color{ 0x0a, 0x0a, 0x0a, 0xff });
        DrawRectangle(9, WINDOW_H - 54, 400 * static_cast<f32>(rover_vel.x * rover_vel.x + rover_vel.y * rover_vel.y) / (MAX_ROVER_VEL * MAX_ROVER_VEL) / 2, 6, Color{ 0x00, 0xff, 0x00, 0xff });

        DrawText("HEADING", 440, WINDOW_H - 46, 30, WHITE);
        DrawRectangle(439, WINDOW_H - 54, 400, 6, Color{ 0x0a, 0x0a, 0x0a, 0xff });
        DrawRectangle(439 + 200 + 199 * ltsinf(rover_angle), WINDOW_H - 54, 10, 6, Color{ 0x00, 0xff, 0x00, 0xff });

        DrawText("HEALTH", 870, WINDOW_H - 46, 30, WHITE);
        DrawRectangle(869, WINDOW_H - 54, 400, 6, Color{ 0x0a, 0x0a, 0x0a, 0xff });
        DrawRectangle(869, WINDOW_H - 54, 400 * world.get_rover().get_health() / Rover::DEFAULT_MAX_HEALTH, 6, Color{ 0x00, 0xff, 0x00, 0xff });
        
        DrawText("MOONCOINS", 1300, WINDOW_H - 54, 30, WHITE);
        DrawText(std::to_string(world.get_collected_mooncoins()).c_str(), 1550, WINDOW_H - 76, 80, WHITE);

        if (world.get_rover().get_health() <= 0.0f) {
            DrawText("GAME OVER", WINDOW_W / 2 - 100, WINDOW_H / 2 - 50, 50, WHITE);
            EndDrawing();
            continue;
        }

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
        util::clamp_lh(rover_vel.x, MIN_ROVER_VEL, MAX_ROVER_VEL);
        util::clamp_lh(rover_vel.y, MIN_ROVER_VEL, MAX_ROVER_VEL);
        util::clamp_lh(rover_angular_vel, -0.1f, 0.1f);
        world.get_rover().set_velocity(rover_vel);
        world.get_rover().set_angular_velocity(rover_angular_vel);

        cam.target(centered_view_of_rover);
        world.step(dt_scale);
        cam.step(dt_scale);
        world.set_position(cam.get());

        world.get_rover().add_health(-0.15f * dt_scale);

        if (GetTime() > next_asteroid_spawn + 1.55f) {
            world.spawn_asteroid_nearby(centered_view_of_rover, 2400.0f);
            next_asteroid_spawn = GetTime();
        }

        if (GetTime() > next_mooncoin_spawn + 0.75f) {
            world.spawn_mooncoin_nearby(centered_view_of_rover, 4000.0f);
            next_mooncoin_spawn = GetTime();
        }
    }

    UnloadSound(theme_bgm);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
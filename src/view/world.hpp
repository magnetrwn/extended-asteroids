#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <vector>

#include "typedef.hpp"
#include "asteroid.hpp"
#include "rover.hpp"
#include "ltmath.hpp"

using namespace LookupTableMath;

/**
 * @brief Main handler for the game world.
 *
 * The World class is responsible for handling all the game's entities and their interactions.
 * It has a circular buffer of asteroids that can be spawned and moved around. By using a circular buffer,
 * the game can start immediately with a set amount of asteroids, and then keep reusing them as they are
 * respawned by the game logic. Also, when coming into view of a hidden asteroid, it will be unculled
 * and start acting normally, creating a more explorable world (perhaps).
 *
 * By changing the type of asteroids, you could technically store everything on the stack.
 *
 * @note The game logic is supposed to spawn (reuse) more asteroids, otherwise during the game cycle, all asteroids
 * will eventually move out of view and never come back.
 */
class World {
private:
    static constexpr usize CIRCULAR_BUFFER_ASTEROIDS = 864;
    static constexpr f32 COLLISION_PUSHBACK = 0.015f;
    static constexpr f32 COLLISION_PUSHBACK_ROVER_V = -2.0f;
    static constexpr f32 CULLING_MARGIN = 1600.0f;
    static constexpr f32 RANDOMIZER_RANGE = 50000.0f;

    struct AsteroidCull {
        Asteroid el;
        bool out_of_view = false;
    };

    std::vector<AsteroidCull> asteroids;
    usize circular_index = 0;
    f32_2 position;
    f32_2 culling_viewport;

    Rover rover;

    void next_index() {
        circular_index = (circular_index + 1) % CIRCULAR_BUFFER_ASTEROIDS;
    }

public:
    World(f32_2 position, f32_2 culling_viewport) : position(position), culling_viewport(culling_viewport) {
        asteroids.resize(CIRCULAR_BUFFER_ASTEROIDS);

        for (usize i = 0; i < get_asteroid_count(); ++i)
            randomize_asteroid(i);
    }

    Rover& get_rover() { return rover; }

    f32_2 get_position() const { return position; }
    void set_position(f32_2 position) { this->position = position; }
    void add_position(f32_2 position) { this->position.x += position.x; this->position.y += position.y; }

    void spawn_asteroid_nearby(f32_2 position, f32 range) {
        const f32 angle = util::randf() * 2.0f * M_PI;

        asteroids[circular_index].el.set_position({ position.x + range * ltcosf_q(angle), position.y + range * ltsinf_q(angle) });
        asteroids[circular_index].el.set_angular_velocity(util::randf() * 0.1f - 0.05f);
        asteroids[circular_index].el.set_velocity({ util::randf() * 2.0f - 1.0f, util::randf() * 2.0f - 1.0f });

        next_index();
    }

    void spawn_asteroid_at(f32_2 position) {
        asteroids[circular_index].el.set_position(position);
        asteroids[circular_index].el.set_angular_velocity(util::randf() * 0.1f - 0.05f);
        asteroids[circular_index].el.set_velocity({ util::randf() * 2.0f - 1.0f, util::randf() * 2.0f - 1.0f });

        next_index();
    }

    void randomize_asteroid(usize index) {
        asteroids[index].el.set_position(
            { 
                util::randf() * RANDOMIZER_RANGE - RANDOMIZER_RANGE / 2, 
                util::randf() * RANDOMIZER_RANGE - RANDOMIZER_RANGE / 2 
            }
        );
        asteroids[index].el.set_angular_velocity(util::randf() * 0.1f - 0.05f);
        asteroids[index].el.set_velocity({ util::randf() * 2.0f - 1.0f, util::randf() * 2.0f - 1.0f });
    
        bool no_collisions = false;

        for (usize attempts = 0; !no_collisions and attempts < 100; ++attempts) {
            no_collisions = true;
            for (usize i = 0; i < get_asteroid_count() - 1; ++i) {
                if (asteroids[index].el.is_collision(asteroids[i].el)) {

                    asteroids[index].el.set_position(
                        { 
                            util::randf() * RANDOMIZER_RANGE - RANDOMIZER_RANGE / 2 + (RANDOMIZER_RANGE / 5) * static_cast<f32>(i), 
                            util::randf() * RANDOMIZER_RANGE - RANDOMIZER_RANGE / 2 + (RANDOMIZER_RANGE / 5) * static_cast<f32>(i)
                        }
                    );
                    asteroids[index].el.set_angular_velocity(util::randf() * 0.1f - 0.05f);
                    asteroids[index].el.set_velocity({ util::randf() * 2.0f - 1.0f, util::randf() * 2.0f - 1.0f });

                    no_collisions = false;
                    break;
                }
            }
        }
    }

    void step(f32 dt_scale) {
        for (usize i = 0; i < get_asteroid_count(); ++i) {
            const f32_2 astpos = asteroids[i].el.get_position();
            asteroids[i].out_of_view = (
                astpos.x < -CULLING_MARGIN + position.x or astpos.x > culling_viewport.x + CULLING_MARGIN + position.x or
                astpos.y < -CULLING_MARGIN + position.y or astpos.y > culling_viewport.y + CULLING_MARGIN + position.y
            );
        }

        for (usize i = 0; i < get_asteroid_count() - 1; ++i) {
            if (asteroids[i].out_of_view)
                continue;

            for (usize j = i + 1; j < get_asteroid_count(); ++j) {
                if (asteroids[j].out_of_view)
                    continue;

                if (asteroids[i].el.is_collision(asteroids[j].el)) {
                    const f32_2 pos_i = asteroids[i].el.get_position();
                    const f32_2 pos_j = asteroids[j].el.get_position();

                    asteroids[i].el.add_position({ (pos_i.x - pos_j.x) * COLLISION_PUSHBACK * dt_scale, (pos_i.y - pos_j.y) * COLLISION_PUSHBACK * dt_scale });
                    asteroids[j].el.add_position({ (pos_j.x - pos_i.x) * COLLISION_PUSHBACK * dt_scale, (pos_j.y - pos_i.y) * COLLISION_PUSHBACK * dt_scale });
                }
            }

            //DrawCircle(pos_r.x, pos_r.y, 5.0f, RED);

            if (rover.is_collision(asteroids[i].el)) {
                const f32_2 pos_i = asteroids[i].el.get_position();
                const f32_2 pos_r = { rover.get_position().x - position.x, rover.get_position().y - position.y };
                const f32_2 vel_i = asteroids[i].el.get_velocity();
                const f32_2 vel_r = rover.get_velocity();

                asteroids[i].el.add_position(
                    { 
                        (pos_i.x - pos_r.x) * COLLISION_PUSHBACK * dt_scale, 
                        (pos_i.y - pos_r.y) * COLLISION_PUSHBACK * dt_scale 
                    }
                );
                rover.add_position(
                    { 
                        (pos_r.x - pos_i.x) * COLLISION_PUSHBACK * dt_scale, 
                        (pos_r.y - pos_i.y) * COLLISION_PUSHBACK * dt_scale
                    }
                );
                rover.add_velocity(
                    { 
                        (vel_r.x - vel_i.x) * COLLISION_PUSHBACK_ROVER_V * dt_scale, 
                        (vel_r.y - vel_i.y) * COLLISION_PUSHBACK_ROVER_V * dt_scale 
                    }
                );

                f32 damage = (vel_i.x * vel_i.x + vel_i.y * vel_i.y - vel_r.x * vel_r.x + vel_r.y * vel_r.y);
                damage *= damage * 0.03f;
                rover.add_health(-damage);
            }
        }

        for (usize i = 0; i < get_asteroid_count(); ++i) {
            if (asteroids[i].out_of_view)
                continue;
            
            asteroids[i].el.step(dt_scale);
        }

        rover.step(dt_scale);
    }

    Asteroid& get_asteroid(usize index) {
        return asteroids[index].el;
    }

    usize get_asteroid_count() const {
        return CIRCULAR_BUFFER_ASTEROIDS;
    }
};

#endif
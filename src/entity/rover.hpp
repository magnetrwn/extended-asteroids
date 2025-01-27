#ifndef ROVER_HPP_
#define ROVER_HPP_

#include "typedef.hpp"
#include "entity.hpp"
#include "ltmath.hpp"

using namespace LookupTableMath;

/**
 * @brief The shape of a rover.
 *
 * Simple struct holding constant values for the rover's shape.
 */
struct RoverShape : public EntityShape {
    void init_shape() {
        f32_2* vertexes = data().vertexes;

        vertexes[0] = { 0.0f, -40.0f };
        vertexes[1] = { -16.0f, -4.0f };
        vertexes[2] = { -32.0f, 32.0f };
        vertexes[3] = { 0.0f, 16.0f };
        vertexes[4] = { 32.0f, 32.0f };
        vertexes[5] = { 16.0f, -4.0f };
    }

    RoverShape() : EntityShape(6) { init_shape(); }
};

/**
 * @brief A rover entity.
 */
class Rover : public Entity {
private:
    RoverShape shape;
    f32_2 triangle_pair[6];
    f32 health;

public:
    static constexpr f32 DEFAULT_MAX_HEALTH = 1000.0f;

    enum Direction {
        UP, DOWN, LEFT, RIGHT
    };

    Rover(f32 health = DEFAULT_MAX_HEALTH) : Entity(&shape), health(health) {}

    f32 get_health() const { return health; }
    void set_health(f32 health) { this->health = health; }
    void add_health(f32 health) { this->health += health; }

    void add_velocity_forward(f32 velocity_mod) {
        const f32 fwd_angle = angle - M_PI / 2.0f;

        add_velocity({ velocity_mod * ltcosf(fwd_angle), velocity_mod * ltsinf(fwd_angle) });
    }

    void dampen_velocity(f32 dampening) {
        velocity.x *= dampening;
        velocity.y *= dampening;
    }

    void dampen_angular_velocity(f32 dampening) {
        angular_velocity *= dampening;
    }

    const f32_2* get_triangle_pair(Direction direction) {
        const f32_2* vtx = rel_vertexes;

        switch (direction) {
        case UP:
            triangle_pair[0] = vtx[0];
            triangle_pair[1] = vtx[1];
            triangle_pair[2] = vtx[3];
            triangle_pair[3] = vtx[0];
            triangle_pair[4] = vtx[5];
            triangle_pair[5] = vtx[3];
            break;
        case DOWN:
            triangle_pair[0] = vtx[1];
            triangle_pair[1] = vtx[2];
            triangle_pair[2] = vtx[3];
            triangle_pair[3] = vtx[5];
            triangle_pair[4] = vtx[4];
            triangle_pair[5] = vtx[3];
            break;
        case LEFT:
            triangle_pair[0] = vtx[0];
            triangle_pair[1] = vtx[1];
            triangle_pair[2] = vtx[3];
            triangle_pair[3] = vtx[2];
            triangle_pair[4] = vtx[3];
            triangle_pair[5] = vtx[1];
            break;
        case RIGHT:
            triangle_pair[0] = vtx[0];
            triangle_pair[1] = vtx[3];
            triangle_pair[2] = vtx[5];
            triangle_pair[3] = vtx[3];
            triangle_pair[4] = vtx[4];
            triangle_pair[5] = vtx[5];
            break;
        }

        return triangle_pair;
    }
};

#endif
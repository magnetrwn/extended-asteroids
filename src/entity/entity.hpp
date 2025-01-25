#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "typedef.hpp"

/**
 * @brief Base class for all entities.
 *
 * The Entity class provides all private members and get/setters for adding position and movement to every entity.
 * By extending this class, the derived class can have a position, velocity, angle, and angular velocity.
 */
class Entity {
protected:
    f32_2 position;
    f32_2 velocity;
    f32 angle;
    f32 angular_velocity;

public:
    Entity(f32_2 position = { 0.0f, 0.0f }, f32_2 velocity = { 0.0f, 0.0f }, f32 angle = 0.0f, f32 angular_velocity = 0.0f) : position(position), velocity(velocity), angle(angle), angular_velocity(angular_velocity) {}

    const f32_2 get_position() const { return position; }
    const f32_2 get_velocity() const { return velocity; }
    const f32 get_angle() const { return angle; }
    const f32 get_angular_velocity() const { return angular_velocity; }

    void set_position(f32_2 position) { this->position = position; }
    void set_velocity(f32_2 velocity) { this->velocity = velocity; }
    void set_angle(f32 angle) { this->angle = angle; }
    void set_angular_velocity(f32 angular_velocity) { this->angular_velocity = angular_velocity; }
};

#endif
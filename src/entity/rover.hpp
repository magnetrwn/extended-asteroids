#ifndef ROVER_HPP_
#define ROVER_HPP_

#include "typedef.hpp"
#include "entity.hpp"

/**
 * @brief The shape of a rover.
 *
 * Simple struct holding constant values for the rover's shape.
 */
struct RoverShape : public EntityShape {
    void init_shape() {
        f32_2* vertexes = data().vertexes;

        vertexes[0] = { 0.0f, 40.0f };
        vertexes[1] = { 32.0f, -32.0f };
        vertexes[2] = { 0.0f, -16.0f };
        vertexes[3] = { -32.0f, -32.0f };
    }

    RoverShape() : EntityShape(4) { init_shape(); }
};

/**
 * @brief A rover entity.
 */
class Rover : public Entity {
private:
    RoverShape shape;

public:
    Rover() : Entity(&shape) {}
};

#endif
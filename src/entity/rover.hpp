#ifndef ROVER_HPP_
#define ROVER_HPP_

#include "typedef.hpp"
#include "entity.hpp"

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

class Rover : public Entity {
private:
    RoverShape shape;

public:
    Rover() : Entity(&shape) {}
};

#endif
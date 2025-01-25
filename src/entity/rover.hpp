#ifndef ROVER_HPP_
#define ROVER_HPP_

#include "typedef.hpp"
#include "entity.hpp"

class Rover : public Entity {
private:
    f32_2 rel_vertexes[4] = {
        { 0.0f, -10.0f },
        { 5.0f, 5.0f },
        { 0.0f, 2.5f },
        { -5.0f, 5.0f }
    };
};

#endif
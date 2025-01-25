#ifndef ROVER_HPP_
#define ROVER_HPP_

#include "typedef.hpp"
#include "entity.hpp"

class Rover : public Entity {
private:
    f32_2 rel_vertexes[4];
};

#endif
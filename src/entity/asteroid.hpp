#ifndef ASTEROID_HPP_
#define ASTEROID_HPP_

#include <cmath>

#include "util.hpp"
#include "typedef.hpp"
#include "entity.hpp"
#include "ltmath.hpp"

using namespace LookupTableMath;

/**
 * @brief The shape of an asteroid.
 *
 * To find the final shape of the asteroid, use the vertexes array to create a polygon.
 * Each pair of floats in the vertexes array is x and y from a zero center.
 *
 * @note Check MAX_VERTEXES for the maximum amount, and the vtx_count field to check how many are actually used in there.
 */
struct AsteroidShape : public EntityShape {
    f32 scale;

    void init_shape() {
        usize vtx_count = data().vtx_count;
        f32_2* vertexes = data().vertexes;

        for (usize i = 0; i < vtx_count; i++) {
            f32 angle = i * (2.0f * M_PI / static_cast<f32>(vtx_count));
            f32 modulo = util::randf() * (scale + 20.0f) + (scale * scale) / 16.0f;

            vertexes[i] = { modulo * ltcosf(angle), modulo * ltsinf(angle) };
        }
    }

    AsteroidShape(usize vtx_count, f32 scale) : EntityShape(vtx_count), scale(scale) {
        init_shape();
    }
};

/**
 * @brief An asteroid entity.
 *
 * The asteroid is a simple entity that has a shape and a position.
 * While the shape data is stored by the AsteroidShape struct, these values aren't used directly.
 * Instead, the Entity this class inherits from uses the shape data to calculate the final relative position of the vertexes.
 */
class Asteroid : public Entity {
private:
    AsteroidShape shape;
    
public:
    Asteroid() : Entity(&shape), shape(util::randi(6, AsteroidShape::MAX_VERTEXES), util::randf() * 50.0f + 5.0f) {}
    Asteroid(usize vtx_count, f32 scale) : Entity(&shape), shape(vtx_count, scale) {}
};

#endif
#ifndef ASTEROID_HPP_
#define ASTEROID_HPP_

#include <stdexcept>
#include <cmath>

#include "util.hpp"
#include "typedef.hpp"

/**
 * @brief The shape of an asteroid.
 *
 * To find the final shape of the asteroid, use the vertexes array to create a polygon.
 * Each pair of floats in the vertexes array is x and y from a zero center.
 *
 * @note Check MAX_VERTEXES for the maximum amount, and the vtx_count field to check how many are actually used in there.
 */
struct AsteroidShape {
    static constexpr usize MAX_VERTEXES = 47;
    static constexpr f32 SCALE = 23.75f;

    usize vtx_count;
    f32_2 vertexes[MAX_VERTEXES];

    void init_shape() {
        for (usize i = 0; i < vtx_count; i++) {
            f32 angle = i * (2.0f * PI / static_cast<f32>(vtx_count));
            f32 modulo = util::randf() * (SCALE + 20.0f) + (SCALE * SCALE) / 16.0f;

            vertexes[i] = { modulo * std::cosf(angle), modulo * std::sinf(angle) };
        }
    }

    AsteroidShape(usize vtx_count) : vtx_count(vtx_count) {
        if (vtx_count > MAX_VERTEXES or vtx_count <= 2)
            throw std::runtime_error("AsteroidShape::AsteroidShape must have 2 < n <= " + std::to_string(MAX_VERTEXES) + " vertexes.");
        
        init_shape();
    }
};

/**
 * @brief An asteroid entity.
 *
 * The asteroid is a simple entity that has a shape and a position.
 * While the shape data is stored by the AsteroidShape struct, these values aren't used directly.
 * Instead, the shape is created by the get_shape_vtx_array method, which returns the vertexes in relative space,
 * based on the Asteroid position, as well as closing the shape by repeating the first vertex at the end.
 */
class Asteroid {
private:
    AsteroidShape shape;
    f32_2 position;
    f32_2 rel_vertexes[AsteroidShape::MAX_VERTEXES + 1]; // Add one vertex to close the drawn shape.
    
public:
    const usize get_shape_vtx_count() const { return shape.vtx_count + 1; }

    const f32_2* get_shape_vtx_array() {
        for (usize i = 0; i < shape.vtx_count; i++)
            rel_vertexes[i] = { shape.vertexes[i].x + position.x, shape.vertexes[i].y + position.y };

        rel_vertexes[shape.vtx_count] = rel_vertexes[0];
        
        return rel_vertexes;
    }

    Asteroid(f32_2 position = VECTOR2ZERO) : shape(util::randi(3, AsteroidShape::MAX_VERTEXES)), position(position) {}
    Asteroid(usize vtx_count, f32_2 position = VECTOR2ZERO) : shape(vtx_count), position(position) {}
};

#endif
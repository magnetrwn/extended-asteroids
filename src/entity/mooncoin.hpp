#ifndef MOONCOIN_HPP_
#define MOONCOIN_HPP_

#include "typedef.hpp"
#include "entity.hpp"

#include "util.hpp"

using namespace LookupTableMath;

/**
 * @brief The shape of a mooncoin.
 *
 * Simple struct holding constant values for the mooncoin's shape.
 */
struct MooncoinShape : public EntityShape {
    void init_shape() {
        f32_2* vertexes = data().vertexes;

        vertexes[0] = { 6.0f, 19.0f };
        vertexes[1] = { 0.0f, 18.0f };
        vertexes[2] = { -6.0f, 14.0f };
        vertexes[3] = { -8.0f, 8.0f };
        vertexes[4] = { -7.0f, 2.0f };
        vertexes[5] = { -2.0f, 4.0f };
        vertexes[6] = { 4.0f, -5.0f };
        vertexes[7] = { 12.0f, -4.0f };
        vertexes[8] = { 16.0f, -2.0f };
        vertexes[9] = { 12.0f, -2.0f };
        vertexes[10] = { 8.0f, 0.0f };
        vertexes[11] = { 5.0f, 2.0f };
        vertexes[12] = { 3.0f, 5.0f };
        vertexes[13] = { 3.0f, 10.0f };
        vertexes[14] = { 4.0f, 14.0f };
        vertexes[15] = { 8.0f, 18.0f };
        vertexes[16] = { 12.0f, 20.0f };
    }

    MooncoinShape() : EntityShape(17) { init_shape(); }
};

/**
 * @brief A mooncoin entity.
 */
class Mooncoin : public Entity {
private:
    MooncoinShape shape;

public:
    static constexpr f32 RECOVERY_AMOUNT = 100.0f;

    Mooncoin() : Entity(&shape) {}
};

#endif
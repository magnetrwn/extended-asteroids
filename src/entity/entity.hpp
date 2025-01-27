#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include "typedef.hpp"
#include "ltmath.hpp"

using namespace LookupTableMath;

/**
 * @brief Base class for all entity vector shapes.
 *
 * The EntityShape struct provides a base for all entities that have a shape made up of vertexes.
 * It has a maximum amount of vertexes and a count of how many are actually used.
 *
 * This struct owns the vertexes array, which can then be passed by the EntityShapeData struct to the Entity.
 *
 * @note To change the point coordinates (like for relative position or rotation), use the Entity's rel_vertexes array.
 */
class EntityShape {
public:
    static constexpr usize MAX_VERTEXES = 31;

private:
    struct EntityShapeData {
        usize vtx_count;
        f32_2* vertexes;
    };

    struct EntityShapeDataConst {
        const usize vtx_count;
        const f32_2* vertexes;
    };

    usize vtx_count;
    f32_2 vertexes[MAX_VERTEXES];

public:
    EntityShapeData data() { return { vtx_count, vertexes }; }
    EntityShapeDataConst data() const { return { vtx_count, vertexes }; }

    EntityShape(usize vtx_count = 0) { this->vtx_count = vtx_count; }
    EntityShape(usize vtx_count, const f32_2* vertexes) : vtx_count(vtx_count) {
        for (usize i = 0; i < vtx_count; ++i)
            this->vertexes[i] = vertexes[i];
    }
    EntityShape(const EntityShapeData& data) : vtx_count(data.vtx_count) {
        for (usize i = 0; i < vtx_count; ++i)
            vertexes[i] = data.vertexes[i];
    }
};

/**
 * @brief Base class for all entities.
 *
 * The Entity class provides all protected members and get/setters for adding position and movement to every entity.
 * It also ties each entity to its shape base class, which provides information on the base vertexes that make it up.
 */
class Entity {
protected:
    EntityShape* shape;
    f32_2 rel_vertexes[EntityShape::MAX_VERTEXES + 1]; // Add one vertex to close the drawn shape.
    f32_2 bounding_box[2];
    f32_2 position;
    f32_2 velocity;
    f32 angle;
    f32 angular_velocity;

    bool ccw(const f32_2& a, const f32_2& b, const f32_2& c) const {
        return (c.y - a.y) * (b.x - a.x) > (b.y - a.y) * (c.x - a.x);
    }

    void update_bounding_box() {
        const usize vtx_count = shape->data().vtx_count;

        bounding_box[0] = rel_vertexes[0];
        bounding_box[1] = rel_vertexes[0];

        for (usize i = 1; i < vtx_count; ++i) {
            bounding_box[0].x = rel_vertexes[i].x < bounding_box[0].x ? rel_vertexes[i].x : bounding_box[0].x;
            bounding_box[0].y = rel_vertexes[i].y < bounding_box[0].y ? rel_vertexes[i].y : bounding_box[0].y;
            bounding_box[1].x = rel_vertexes[i].x > bounding_box[1].x ? rel_vertexes[i].x : bounding_box[1].x;
            bounding_box[1].y = rel_vertexes[i].y > bounding_box[1].y ? rel_vertexes[i].y : bounding_box[1].y;
        }
    }

public:
    Entity(EntityShape* shape, f32_2 position = { 0.0f, 0.0f }, f32_2 velocity = { 0.0f, 0.0f }, f32 angle = 0.0f, f32 angular_velocity = 0.0f)
      : shape(shape), position(position), velocity(velocity), angle(angle), angular_velocity(angular_velocity) {}

    const f32_2 get_position() const { return position; }
    const f32_2 get_velocity() const { return velocity; }
    const f32 get_angle() const { return angle; }
    const f32 get_angular_velocity() const { return angular_velocity; }

    void set_position(f32_2 position) { this->position = position; }
    void set_velocity(f32_2 velocity) { this->velocity = velocity; }
    void set_angle(f32 angle) { this->angle = angle; }
    void set_angular_velocity(f32 angular_velocity) { this->angular_velocity = angular_velocity; }

    void add_position(f32_2 position) { this->position.x += position.x; this->position.y += position.y; }
    void add_velocity(f32_2 velocity) { this->velocity.x += velocity.x; this->velocity.y += velocity.y; }
    void add_angle(f32 angle) { this->angle += angle; }
    void add_angular_velocity(f32 angular_velocity) { this->angular_velocity += angular_velocity; }

    usize get_entity_vtx_count() const { return shape->data().vtx_count + 1; }

    const f32_2* get_entity_vtx_array() {
        return rel_vertexes;
    }

    void step(f32 dt_scale) {
        position.x += velocity.x * dt_scale;
        position.y += velocity.y * dt_scale;
        angle += angular_velocity * dt_scale;

        update_bounding_box();
        //DrawRectangleLines(bounding_box[0].x, bounding_box[0].y, bounding_box[1].x - bounding_box[0].x, bounding_box[1].y - bounding_box[0].y, RED);
    
        const usize vtx_count = shape->data().vtx_count;
        const f32_2* vertexes = shape->data().vertexes;

        float sin_angle = ltsinf(angle);
        float cos_angle = ltcosf(angle);

        for (usize i = 0; i < vtx_count; ++i) {
            float rx = vertexes[i].x * cos_angle - vertexes[i].y * sin_angle;
            float ry = vertexes[i].x * sin_angle + vertexes[i].y * cos_angle;
            rel_vertexes[i] = { rx + position.x, ry + position.y };
        }
        
        rel_vertexes[vtx_count] = rel_vertexes[0];
    }

    bool is_collision(const Entity& other) const {
        if (this == &other)
            return false;

        bool bounding_box_collision = (
            bounding_box[0].x < other.bounding_box[1].x and
            bounding_box[1].x > other.bounding_box[0].x and
            bounding_box[0].y < other.bounding_box[1].y and
            bounding_box[1].y > other.bounding_box[0].y
        );

        if (!bounding_box_collision)
            return false;

        const usize vtx_count[2] = { shape->data().vtx_count, other.shape->data().vtx_count };
        const f32_2* vertexes[2] = { shape->data().vertexes, other.shape->data().vertexes };

        for (usize i = 0; i < vtx_count[0]; ++i) {
            const f32_2 p1 = rel_vertexes[i];
            const f32_2 p2 = rel_vertexes[i + 1];

            for (usize j = 0; j < vtx_count[1]; ++j) {
                const f32_2 q1 = other.rel_vertexes[j];
                const f32_2 q2 = other.rel_vertexes[j + 1];

                if (ccw(p1, q1, q2) != ccw(p2, q1, q2) and ccw(p1, p2, q1) != ccw(p1, p2, q2))
                    return true;
            }
        }

        return false;
    }
};

#endif
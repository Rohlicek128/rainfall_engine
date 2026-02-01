#pragma once

#include "engine/world/physics/CollisionPoints.h"

class RidgidbodyCollider;

namespace engine::physics
{
    struct Collision
    {
        RidgidbodyCollider* body_a;
        RidgidbodyCollider* body_b;
        CollisionsPoints points;
    };
}

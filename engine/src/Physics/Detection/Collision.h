#pragma once

#include "engine/world/physics/CollisionPoints.h"
#include "RidgidbodyCollider.h"

namespace engine::physics
{
    struct Collision
    {
        RidgidbodyCollider* body_a;
        RidgidbodyCollider* body_b;
        CollisionsPoints points;
    };
}

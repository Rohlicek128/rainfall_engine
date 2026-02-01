#pragma once

#include "engine/world/components/RidgidbodyComponent.h"
#include "engine/world/components/Collider.h"


namespace engine::physics
{
    struct RidgidbodyCollider
    {
        RidgidbodyComponent* ridgidbody;
        Collider* collider;
    };
}

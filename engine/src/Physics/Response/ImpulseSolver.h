#pragma once

#include "engine/world/physics/Solver.h"


namespace engine::physics
{
    class ImpulseSolver : public Solver
    {
    public:
        void solve(std::vector<Collision>& collisions, float delta_time) override;
    };
}

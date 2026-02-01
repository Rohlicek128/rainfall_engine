#pragma once

#include <vector>


namespace engine::physics
{
    class Collision;

    class Solver
    {
    public:
        virtual void solve(std::vector<Collision>& collisions, float delta_time) = 0;
    };
}

#include "ImpulseSolver.h"

#include "../Detection/Collision.h"


namespace engine::physics
{
    // Taken from https://github.com/IainWinter/IwEngine/blob/3e2052855fea85718b7a499a7b1a3befd49d812b/IwEngine/src/physics/Dynamics/ImpulseSolver.cpp
    void ImpulseSolver::solve(std::vector<Collision>& collisions, float delta_time)
    {
        for (Collision& collision : collisions) {

			RidgidbodyComponent* aBody = collision.body_a->ridgidbody->is_dynamic ? collision.body_a->ridgidbody : nullptr;
			RidgidbodyComponent* bBody = collision.body_b->ridgidbody->is_dynamic ? collision.body_b->ridgidbody : nullptr;

			float aInvMass = (aBody && aBody->is_dynamic) ? 1.0f / aBody->mass : 0.0f;
            float bInvMass = (bBody && bBody->is_dynamic) ? 1.0f / bBody->mass : 0.0f;

            if (aInvMass + bInvMass == 0.0f) continue; // Both are static

            glm::vec3 rVel = (bBody ? bBody->velocity : glm::vec3(0)) - (aBody ? aBody->velocity : glm::vec3(0));
			float nSpd = glm::dot(rVel, collision.points.normal);

			// Impulse

			// This is important for convergence
			// a negitive impulse would drive the objects closer together
			if (nSpd >= 0)
				continue;

			float e = (aBody ? aBody->restitution : 1.0f) * (bBody ? bBody->restitution : 1.0f);
			float j = -(1.0f + e) * nSpd / (aInvMass + bInvMass);

			glm::vec3 impluse = j * collision.points.normal;

			if (aBody && aBody->is_simulated) aBody->velocity -= impluse * aInvMass;
            if (bBody && bBody->is_simulated) bBody->velocity += impluse * bInvMass;

			// Friction

			rVel = (bBody ? bBody->velocity : glm::vec3(0)) - (aBody ? aBody->velocity : glm::vec3(0));
			nSpd = glm::dot(rVel, collision.points.normal);

			glm::vec3 tangent = rVel - glm::dot(rVel, collision.points.normal) * collision.points.normal;

			if (glm::length(tangent) > 0.0001f) {
				tangent = glm::normalize(tangent);

				float fVel = glm::dot(rVel, tangent);
				float mu  = glm::length(glm::vec2(aBody ? aBody->static_friction : 0, bBody ? bBody->static_friction : 0));
				float f  = -fVel / (aInvMass + bInvMass);

				glm::vec3 friction;
				if (abs(f) < j * mu) {
					friction = f * tangent;
				}
				else {
					mu = glm::length(glm::vec2(aBody ? aBody->dynamic_friction : 0, bBody ? bBody->dynamic_friction : 0));
					friction = -j * tangent * mu;
				}

				if (aBody && aBody->is_simulated) aBody->velocity -= friction * aInvMass;
                if (bBody && bBody->is_simulated) bBody->velocity += friction * bInvMass;
			}
		}
    }

}

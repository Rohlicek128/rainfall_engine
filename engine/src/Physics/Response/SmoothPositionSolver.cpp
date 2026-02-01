#include "SmoothPositionSolver.h"

#include "../Detection/Collision.h"


namespace engine::physics
{
    // Taken from https://github.com/IainWinter/IwEngine/blob/3e2052855fea85718b7a499a7b1a3befd49d812b/IwEngine/src/physics/Dynamics/SmoothPositionSolver.cpp
    void SmoothPositionSolver::solve(std::vector<Collision>& collisions, float delta_time)
    {
        //std::vector<std::pair<glm::vec3, glm::vec3>> deltas;

		for (Collision& collision : collisions) {
		    RidgidbodyComponent* aBody = collision.body_a->ridgidbody->is_dynamic ? collision.body_a->ridgidbody : nullptr;
			RidgidbodyComponent* bBody = collision.body_b->ridgidbody->is_dynamic ? collision.body_b->ridgidbody : nullptr;

			float aInvMass = (aBody && aBody->is_dynamic) ? 1.0f / aBody->mass : 0.0f;
            float bInvMass = (bBody && bBody->is_dynamic) ? 1.0f / bBody->mass : 0.0f;

            if (aInvMass + bInvMass == 0.0f) continue;

			const float percent = 0.4f;
			const float slop = 0.01f;

			glm::vec3 correction = collision.points.normal *
			    (std::max(collision.points.depth - slop, 0.0f) / (aInvMass + bInvMass))
			    * percent;

            if (aBody && aBody->is_simulated) {
                aBody->transform->position -= correction * aInvMass;
            }
            if (bBody && bBody->is_simulated) {
                bBody->transform->position += correction * bInvMass;
            }
		}

		/*for (unsigned i = 0; i < collisions.size(); i++) {
			RidgidbodyComponent* aBody = collisions[i].body_a->ridgidbody->is_dynamic ? collisions[i].body_a->ridgidbody : nullptr;
			RidgidbodyComponent* bBody = collisions[i].body_b->ridgidbody->is_dynamic ? collisions[i].body_b->ridgidbody : nullptr;

			if (aBody ? aBody->is_simulated : false) {
				aBody->transform->position -= deltas[i].first;
			}

			if (bBody ? bBody->is_simulated : false) {
				bBody->transform->position += deltas[i].second;
			}
		}*/
    }

}

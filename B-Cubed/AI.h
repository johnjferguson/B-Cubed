#pragma once
#include <vector>
#include "physx/include/PxPhysicsAPI.h"
#include <DirectXMath.h>

class AI {
	private:
		// Vehicle Reference?

		// Path
		std::vector<physx::PxVec3> path; // First point should be first goal
		int targetPoint = 0; // Index for path to get a goal point

		// Tweakable Variables
		float safeRange = 20.f; // Distance from point that considers reaching said point
		float slowRange = 30.f; // Distance from point where AI will drive slower (to not drive past)
		float directionRange = 10.f; // 10 Degrees?

		// Output
		bool accel = false;
		bool brake = false;
		float steer = 0.f;

	public:
		// Constructor/Deconstructor
		AI();
		AI(std::vector<physx::PxVec3> p);
		~AI();

		// Function to update AI input data
		void update(physx::PxVec3 position, physx::PxVec3 velocity, DirectX::XMVECTOR direction);

		bool getAcceleration();
		bool getBrake();
		float getSteering();
	private:
		float distanceToPoint(physx::PxVec3 position, physx::PxVec3 goal);
		float directionToPoint(physx::PxVec3 position, physx::PxVec3 goal);
		float vectorToDegrees(DirectX::XMVECTOR dir);
};
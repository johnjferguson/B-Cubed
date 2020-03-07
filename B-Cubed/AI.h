#pragma once
#include <vector>
#include "physx/include/PxPhysicsAPI.h"
#include <DirectXMath.h>

class AI {
	private:
		// AI States
		enum aistate {
			DRIVE, TURN, REVERSE
		};

		aistate state = TURN;

		// Path
		std::vector<physx::PxVec3> path; // Path that the AI should "follow"
		int targetPoint = 0; // Index for path to get a goal point
		physx::PxVec3 point; // The point that the AI is trying to get to.
		

		physx::PxVehicleDrive4W* car; // Reference to the car this AI controls

		

		// Tweakable Variables
		float safeRange = 20.f; // Distance from point that considers reaching said point
		float slowRange = 30.f; // Distance from point where AI will drive slower (to not drive past/make turning easier)
		float directionRange = 10.f; // 10 Degrees?

		// Car Stats?
		float speed = 0.f; // Current speed calculated from velocity vector
		float rpm = 0.f; // Current ENGINE RPM
		float distance = 0.f; // Distance from current position to target point
		
		float degToPoint = 0.f; // Degrees to point
		float degFacing = 0.f; // Current degrees facing
		float dirDiff = 0.f; // The difference between the above 2 degrees


		// Output
		bool accel = false;
		bool brake = false;
		float steer = 0.f;

	public:
		// Constructor/Deconstructor
		AI();
		AI(std::vector<physx::PxVec3> p, physx::PxVehicleDrive4W* c);
		~AI();

		// Function to update AI input data
		void update();

		bool getAcceleration();
		bool getBrake();
		float getSteering();

	private:
		void doCalculations();
		bool isStuck();

		float distanceToPoint(physx::PxVec3 position, physx::PxVec3 goal);
		float directionToPoint(physx::PxVec3 position, physx::PxVec3 goal);
		float vectorToDegrees(DirectX::XMVECTOR dir);

		void drive();
		void turn();
		void reverse();
};
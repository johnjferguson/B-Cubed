#pragma once
#include <vector>
#include "physx/include/PxPhysicsAPI.h"
#include <DirectXMath.h>
#include <stdlib.h> // rand

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
		int safeRange = 26; // Distance from point that considers reaching said point
		int slowRange = 0; // Distance from point where AI will drive slower (to not drive past/make turning easier)
		int directionRange = 4; // Maximal degrees error margin
		float steerSpeed = 0.8f; // How much the wheels turn [0.f, 1.f]
		int turnSpeed = rand() % 20 + 40; // Max speed for turning (AI will brake if above) (50 is good)

		// Abilities
		int abilityCD = 60;
		int currCD = 0;

		int boostRange = rand() % 30 + 70; // Distance from point for AI to boost 80
		int boostChanceDrive = rand() % 80 + 20;
		int boostChanceTurn = rand() % 20;


		// Chance to use ability = i / sum
		// [0] - boost
		// [1] - barrier
		// [2] - blast
		int abilities[3] = { 1, 1, 1 };

		// Car Stats
		float speed = 0.f; // Current speed calculated from velocity vector
		float rpm = 0.f; // Current ENGINE RPM
		float distance = 0.f; // Distance from current position to target point
		
		float degToPoint = 0.f; // Degrees to point
		float degFacing = 0.f; // Current degrees facing
		float dirDiff = 0.f; // The difference between the above 2 degrees

		int stateLock = 0; // Gets set above 0 to stay in the state for x amount of update calls
		int sleepTime = rand() % 80;

		// Output
		bool accel = false;
		bool brake = false;
		float steer = 0.f;

		bool boost = false;
		bool barrier = false;
		bool blast = false;

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

		bool getBoost();
		bool getBarrier();
		bool getBlast();

	private:
		void doCalculations();
		void selectState();
		void abilitySelect();
		bool isStuck();

		void nextPoint();
		void fetchPoint();

		float distanceToPoint(physx::PxVec3 position, physx::PxVec3 goal);
		float directionToPoint(physx::PxVec3 position, physx::PxVec3 goal);
		float vectorToDegrees(DirectX::XMVECTOR dir);

		void drive();
		void turn();
		void reverse();
};
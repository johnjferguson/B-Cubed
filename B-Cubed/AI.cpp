#include "AI.h";
#include "Gui.h"

AI::AI() {

}

AI::AI(std::vector<physx::PxVec3> p) {
	path = p;
}

AI::~AI() {

}

float AI::distanceToPoint(physx::PxVec3 position, physx::PxVec3 goal) {
	float x = position.x;
	float y = position.y;
	float z = position.z;

	float px = goal.x;
	float py = goal.y;
	float pz = goal.z;

	return pow(pow(px - x, 2) + pow(py - y, 2) + pow(pz - z, 2), 0.5f);
}

float AI::directionToPoint(physx::PxVec3 position, physx::PxVec3 goal) {
	// Only want 2D direction
	float x = position.x;
	float z = position.z;

	float px = goal.x;
	float pz = goal.z;

	float dx = px - x;
	float dz = pz - z;

	float deg = atan(dz / dx) * 180.f / 3.14159f;

	if (dx < 0)
		deg += 180;
	else if (dx > 0 && dz < 0)
		deg += 360;

	return deg;
}

float AI::vectorToDegrees(DirectX::XMVECTOR dir) {
	float x = DirectX::XMVectorGetX(dir);
	float z = DirectX::XMVectorGetZ(dir);

	float deg = atan(z / x) * 180.f / 3.14159f;
	if (x < 0)
		deg += 180;
	else if (x > 0 && z < 0)
		deg += 360;

	return deg;
}

void AI::update(physx::PxVec3 position, physx::PxVec3 velocity, DirectX::XMVECTOR direction) {
	// Clear all current inputs
	accel = false;
	brake = false;
	steer = 0.f;

	float speed = pow(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2), 0.5f);

	if (path.size() > 0) {
		physx::PxVec3 point = path[targetPoint];

		// If Vehicle is within an adequate distance to its targeted point
		float distance = distanceToPoint(position, point);
		Gui::AddText("Distance: " + std::to_string(distance));

		if (distance <= safeRange) {
			targetPoint += 1;

			if (targetPoint >= path.size())
				targetPoint = 0;

			// Update point after finding new target
			point = path[targetPoint];
		}

		// Ensure car is facing point
		float deg = directionToPoint(position, point);
		float facing = vectorToDegrees(direction);
		float dd = deg - facing; // Direction difference
		if (dd < -180)
			dd += 360;
		else if (dd > 180)
			dd -= 360;
		
		Gui::AddText("Speed: " + std::to_string(speed));

		// Is car direction outside of allowed range
		if (abs(dd) > directionRange) {
			if (dd > 0) {
				// GOAL: Decrease direction
				// Requires tweaking
				accel = true;
				steer = -0.4f;
			}
			else {
				// GOAL: Increase direction

				accel = true;
				steer = 0.4f;
			}
		} else {
			//
			// AT THIS POINT THE CAR SHOULD BE FACING THE POINT WITHIN ALLOWED RANGE
			//

			// Calculate Movement
			if (distanceToPoint(position, point) > slowRange) {
				// GOAL: Go fast

				accel = true;
			} else {
				// GOAL: Slowdown to adequate speed for turning before entering safeRange

				if (speed < 4)
					accel = true;
			}
		}
	}
}

bool AI::getAcceleration() {
	return accel;
}

bool AI::getBrake() {
	return brake;
}

float AI::getSteering() {
	return steer;
}
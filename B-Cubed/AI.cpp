#include "AI.h"
#include "Gui.h"

AI::AI() {

}

AI::AI(std::vector<physx::PxVec3> p, physx::PxVehicleDrive4W* c) {
	path = p;
	car = c;
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

void AI::doCalculations() {
	physx::PxVec3 velocity = car->getRigidDynamicActor()->getLinearVelocity();
	physx::PxVec3 position = car->getRigidDynamicActor()->getGlobalPose().p;
	physx::PxQuat quint = car->getRigidDynamicActor()->getGlobalPose().q;
	DirectX::XMVECTOR direction = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)).r[2];
	
	
	speed = car->computeForwardSpeed(); //pow(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2), 0.5f);
	rpm = car->mDriveDynData.getEngineRotationSpeed();

	if (path.size() > 0) {
		point = path[targetPoint];

		// Calculate distance to point
		distance = distanceToPoint(position, point);

		// Calculate direction variables
		degToPoint = directionToPoint(position, point);
		degFacing = vectorToDegrees(direction);
		dirDiff = degToPoint - degFacing;
		if (dirDiff < -180) {
			dirDiff += 360;
		} else if (dirDiff > 180) {
			dirDiff -= 360;
		}
	}
}

void AI::update() {
	// Clear all current inputs
	accel = false;
	brake = false;
	steer = 0.f;

	doCalculations();

	//Gui::AddText("AI Speed: " + std::to_string(speed) );

	switch (state) {
		case DRIVE:
			Gui::AddText("Current State: DRIVE");
			drive();
			break;
		case TURN:
			Gui::AddText("Current State: TURN");
			turn();
			break;
		case REVERSE:
			Gui::AddText("Current State: REVERSE");
			reverse();
			break;
		default:
			break;
	}
}

void AI::drive() {
	if (distance <= safeRange) {
		targetPoint += 1;

		if (targetPoint >= path.size()) {
			targetPoint = 0;
		}

		state = TURN;

	} else {
		if (distance > slowRange) {
			// GOAL: Go fast
			accel = true;
		} else {
			// GOAL: Slowdown to adequate speed for turning before entering safeRange
			if (speed < 4) {
				accel = true;
			}
		}

		if (isStuck()) {
			state = REVERSE;
		}
	}
}

bool AI::isStuck() {
	return (rpm > 90 && abs(speed) < 0.1f);
}

void AI::turn() {
	accel = true;
	if (abs(dirDiff) > directionRange) {
		if (dirDiff > 0) {
			// GOAL: Decrease direction
			steer = -0.4f;
		}
		else {
			// GOAL: Increase direction
			steer = 0.4f;
		}

		if (isStuck()) {
			state = REVERSE;
		}
	} else {
		state = DRIVE;
	}
}

void AI::reverse() {
	brake = true;

	if (abs(dirDiff) > directionRange) {
		if (dirDiff > 0) {
			// GOAL: Decrease direction
			steer = 0.4f;
		}
		else {
			// GOAL: Increase direction
			steer = -0.4f;
		}
	}
	else {
		state = DRIVE;
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
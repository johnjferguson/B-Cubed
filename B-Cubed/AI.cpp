#include "AI.h"
#include "Gui.h"

#include <stdlib.h> // rand
#include <sstream> // stringstream

AI::AI() {

}

AI::AI(std::vector<physx::PxVec3> p, physx::PxVehicleDrive4W* c) {
	path = p;
	car = c;

	fetchPoint();
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

	return (float) pow(pow(px - x, 2) + pow(py - y, 2) + pow(pz - z, 2), 0.5f);
}

float AI::directionToPoint(physx::PxVec3 position, physx::PxVec3 goal) {
	// Only want 2D direction
	float x = position.x;
	float z = position.z;

	float px = goal.x;
	float pz = goal.z;

	float dx = px - x;
	float dz = pz - z;

	float deg = (float) atan(dz / dx) * 180.f / 3.14159f;

	if (dx < 0)
		deg += 180;
	else if (dx > 0 && dz < 0)
		deg += 360;

	return deg;
}

float AI::vectorToDegrees(DirectX::XMVECTOR dir) {
	float x = DirectX::XMVectorGetX(dir);
	float z = DirectX::XMVectorGetZ(dir);

	float deg = (float) atan(z / x) * 180.f / 3.14159f;
	if (x < 0)
		deg += 180;
	else if (x > 0 && z < 0)
		deg += 360;

	return deg;
}

void AI::nextPoint() {
	targetPoint += 1;

	if (targetPoint >= path.size()) {
		targetPoint = 0;
	}

	fetchPoint();
}

void AI::fetchPoint() {
	point = path[targetPoint];
	//point.x += rand() % 5 - 2; // Add random range [-2, 2]
	//point.z += rand() % 5 - 2; // Add random range [-2, 2]
}

void AI::doCalculations() {
	physx::PxVec3 velocity = car->getRigidDynamicActor()->getLinearVelocity();
	physx::PxVec3 position = car->getRigidDynamicActor()->getGlobalPose().p;
	physx::PxQuat quint = car->getRigidDynamicActor()->getGlobalPose().q;
	DirectX::XMVECTOR direction = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(quint.x, quint.y, quint.z, quint.w)).r[2];
	
	
	speed = car->computeForwardSpeed(); //pow(pow(velocity.x, 2) + pow(velocity.y, 2) + pow(velocity.z, 2), 0.5f);
	rpm = car->mDriveDynData.getEngineRotationSpeed();

	if (path.size() > 0) {
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

void AI::selectState() {
	if (stateLock > 0) {
		stateLock -= 1;
	} else {
		if (isStuck() && state != REVERSE) {
			stateLock = 120; // ~60/sec
			state = REVERSE;
		} else {
			if (abs(dirDiff) > directionRange) {
				state = TURN;
				stateLock = 4;
			}
			else {
				state = DRIVE;
				stateLock = 4;
			}
		}
	}
	

	std::stringstream ss;
	ss << "AI(speed=" << speed << ", " << "stateLock=" << stateLock << ", state=" << state << ", point=" << (int)point.x << ", " << (int)point.y << ", " << (int)point.z << ")";
	Gui::AddText(ss.str().c_str());
}

void AI::abilitySelect() {
	if (currCD > 0) {
		currCD -= 1;
	} else {
		int total = abilities[0] + abilities[1] + abilities[2];
		if (total > 0) {
			int k = rand() % total + 1;

			if (k <= abilities[0]) {
				boost = true;
			}
			else if (k > abilities[0] && k <= abilities[0] + abilities[1]) {
				barrier = true;
			}
			else {
				blast = true;
			}

			currCD = abilityCD;
		}
	}
}

void AI::update() {
	// Clear all current inputs
	accel = false;
	brake = false;
	steer = 0.f;

	boost = false;
	barrier = false;
	blast = false;

	abilities[0] = 0; // Boost
	abilities[1] = 10; // Barrier
	abilities[2] = 5; // Blast

	// Do Brain stuff
	doCalculations();
	selectState();

	if (distance <= safeRange) {
		nextPoint();
	}

	switch (state) {
		case DRIVE:
			drive();
			break;
		case TURN:
			turn();
			break;
		case REVERSE:
			reverse();
			break;
		default:
			break;
	}

	
	abilitySelect();

}

void AI::drive() {
	accel = true;
	
	if (distance < slowRange && speed > 30) {
		// GOAL: Slowdown to adequate speed for turning before entering safeRange
		accel = false;
	} else if (distance > boostRange) {
		abilities[0] = 90;
	}


}

bool AI::isStuck() {
	return (rpm > 90 && abs(speed) < 0.1f);
}

void AI::turn() {

	if (speed > turnSpeed) {
		brake = true;
	} else {
		accel = true;
	}
	
	if (dirDiff > 0) {
		// GOAL: Decrease direction
		steer = -steerSpeed;
	} else {
		// GOAL: Increase direction
		steer = steerSpeed;
	}
}

void AI::reverse() {
	abilities[2] = 0;
	brake = true;

	if (dirDiff > 0) {
		// GOAL: Increase direction
		steer = steerSpeed;
	} else {
		// GOAL: Decrease direction
		steer = -steerSpeed;
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

bool AI::getBoost() {
	return boost;
}

bool AI::getBarrier() {
	return barrier;
}

bool AI::getBlast() {
	return blast;
}
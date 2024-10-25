#ifndef VEHICLE_H
#define VEHICLE_H

#include "raylib.h"
#include "raymath.h"

typedef struct Vehicle {
	Matrix transform;
	Vector3 vel;
} Vehicle;

void VehicleControl(Vehicle* v)
{
	float deltaTime = GetFrameTime();

		/* INPUTS */
	float throttle 	= IsKeyDown(KEY_W);
	float brake 	= IsKeyDown(KEY_S);
	float steer 	= IsKeyDown(KEY_A) - IsKeyDown(KEY_D);

		/* VEHICLE STATS */
	const float MAX_ACCEL = 8.0f;
	const float MAX_STEER = PI/24.0f;
	const float MAX_GRIP = 14.0f; // m/s^2

		/* INTERMEDIATE DATA TYPES */
	Matrix t = v->transform;

	// Vector3 xBasis = { t.m0, t.m1, t.m2 };
	// Vector3 yBasis = { t.m4, t.m5, t.m6 };
	Vector3 zBasis = { t.m8, t.m9, t.m10};

	Vector3 pos = { t.m12, t.m13, t.m14};
	Vector3 vel = v->vel; // just so its consistent! 

	Quaternion rotation = QuaternionFromMatrix(t);

	float speed = Vector3Length(vel);
	float yaw = atan2f(zBasis.x, zBasis.z);

	float dot = Vector3DotProduct(zBasis, vel);
	int direction = (signbit(dot) ? -1 : 1);
	speed *= direction;

		/* ACCEL PHYSICS */
	float engineAccel = deltaTime * throttle * MAX_ACCEL;
	float brakeAccel = deltaTime * direction*brake * MAX_ACCEL; // TODO create variable for brake force!
	float gripAccel = zBasis.x*vel.z - zBasis.z*vel.x; // multiplying by velocity means its already deltaTime'd

	Vector3 accel = { gripAccel, 0.0f, engineAccel - brakeAccel };
	if(Vector3Length(accel) > MAX_GRIP) {
		accel = Vector3Normalize(accel);
		accel = Vector3Scale(accel, MAX_GRIP);
	}

	accel = Vector3RotateByQuaternion(accel, rotation);

	vel = Vector3Add(vel, accel); //update velocity

		/* STEERING */
	float deltaYaw = steer*speed*MAX_STEER;
	Matrix newTransform = MatrixRotateY(yaw + deltaYaw * deltaTime);

		/* CURSED MATRIX MATH */
	newTransform.m12 = pos.x + vel.x*deltaTime;
	newTransform.m13 = pos.y + vel.y*deltaTime;
	newTransform.m14 = pos.z + vel.z*deltaTime;

		/* SIDE-EFFECT */
	v->transform = newTransform;
	v->vel = vel;

	return;
}

Camera VehicleCameraUpdate(Vehicle* v)
{
	Camera c = {
		.position = (Vector3){ 0.0f, 4.0f, -8.0f },
		.target = (Vector3){ 0.0f, 1.0f, 0.0f },
		.up = (Vector3){ 0.0, 1.0f, 0.0f },
		.fovy = 45.0f,
		.projection = CAMERA_PERSPECTIVE
	};

	Matrix t = v->transform;
	Vector3 position = { t.m12, t.m13, t.m14};
	Quaternion rotation = QuaternionFromMatrix(t);

	c.target = Vector3Add(c.target, position);
	c.position = Vector3RotateByQuaternion(c.position, rotation);
	c.position = Vector3Add(c.position, position);

	return c;
}
#endif /* VEHICLE_H */

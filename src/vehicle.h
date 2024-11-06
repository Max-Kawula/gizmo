/************************\
||	vehicle.h	||
\************************/

//by Max Kawula

#ifndef VEHICLE_H
#define VEHICLE_H

#include "raylib.h"
#include "raymath.h"
#include "carspecs.h"

// this is kind of a vector3 in disguise!
typedef struct InputField {
	float thrust;
	float brake;
	float steer;
} InputField;

typedef struct Vehicle {
	Matrix origin;
	Vector3 vel;
	Quaternion angularVel;
	InputField input;
} Vehicle;

int VehicleSpeedSign(Vehicle* v)
{
	Matrix t = v->origin;
 	Vector3 forward = { t.m8, t.m9, t.m10};

 	float dot = Vector3DotProduct(forward, v->vel);
 	return (signbit(dot) ? -1 : 1);
}

Vector3 MatrixBasisVector(Matrix* matrix, int v)
{
	Vector3 basis = { 0 };
	float* m = (float*)matrix;

	basis.x = *(m + 0 + v);
	basis.y = *(m + 4 + v);
	basis.z = *(m + 8 + v);
	
	return basis;
}

float VehicleLongForce(Vehicle* v)
{
		/* CONSTANTS */
	const float ENGINE_THRUST = 2000.0f; //newtons
	const float BRAKE_FORCE = 5000.0f; //newtons

	const float C_RR = 10.5f;

		/* FORCE CALC */
	InputField in = v->input;

 	Vector3 forward = MatrixBasisVector(&v->origin, 2);

 	float dot = Vector3DotProduct(forward, v->vel);
 	int dir = (signbit(dot) ? -1 : 1);

	float forceThrust = in.thrust * ENGINE_THRUST;
	float forceBrake  = -in.brake * dir * BRAKE_FORCE;
	float forceRR	  = dot * -C_RR;

	float forceTotal = forceThrust + forceBrake + forceRR;

	return forceTotal;
}

float VehicleLatForce(Vehicle* v)
{
 	Vector3 right = MatrixBasisVector(&v->origin, 0);
	float sideForce = Vector3DotProduct(right, v->vel);

	return -sideForce;
}
// just want to note that this is very inaccurate, basically setting the position
void VehicleRotate(Vehicle* v)
{
	float deltaTime = GetFrameTime();

	const float MAX_STEER = PI*0.75; //60 degrees
	float deltaYaw = v->input.steer * MAX_STEER; //delta is term for wheel's rotation around car's axis

	v->origin = MatrixMultiply(MatrixRotateY(deltaYaw*deltaTime), v->origin);

	return;
}


Vector3 VehicleDragForce(Vehicle* v);

void VehicleMonoUpdate(Vehicle* v)
{
	const float deltaTime = GetFrameTime();

 		/* INPUTS */
	v->input.thrust = IsKeyDown(KEY_W);
	v->input.brake	= IsKeyDown(KEY_S);
	v->input.steer	= IsKeyDown(KEY_A) - IsKeyDown(KEY_D);


		/* VEHICLE STATE */
 	Vector3 forward = MatrixBasisVector(&v->origin, 2);
 	Vector3 right = MatrixBasisVector(&v->origin, 0);
	
		/* SPACE-SHIP STEERING */
	VehicleRotate(v);

		/* CALC FORCES */
	float forceLong = VehicleLongForce(v);
	float forceLat = VehicleLatForce(v);

		/* UPDATE ACCEL */
	Vector3 forceLongV = Vector3Scale(forward, forceLong);
	Vector3 forceLatV = Vector3Scale(right, forceLat);

	Vector3 accel = Vector3Scale(forceLongV, 1.0f/975.0f); // FORCE / MASS (using a literal for now)
	accel = Vector3Add(accel, forceLatV);

		/* UPDATE VELOCITY */
	v->vel = Vector3Add(v->vel, Vector3Scale(accel, deltaTime));

		/* UPDATE POSITION */
	Vector3 translation = Vector3Scale(v->vel, deltaTime);
	v->origin.m12 += translation.x;
	v->origin.m13 += translation.y;
	v->origin.m14 += translation.z;



	return;
}

/*
 * leaving this here so i can reference the gear box calculations
 */
// void VehicleControl(Vehicle* v)
// {
// 	CarSpec car = ae86; //this should fucking work
// 
// 	float driveForce = car.engineTorque * car.gearRatio[1] * car.diffRatio / car.wheelRadius;
// 	float maxForce = car.mu * car.mass * 9.81f;
// 
// 	float deltaTime = GetFrameTime();
// 
//  		/* INPUTS */
// 	float inputThrottle 	= IsKeyDown(KEY_W);
// 	float inputBrake 	= IsKeyDown(KEY_S);
// 	//float inputSteer 	= IsKeyDown(KEY_A) - IsKeyDown(KEY_D);
// 
// 		/* VEHICLE STATE */
// 	Matrix t = v->origin;
//  	Vector3 forward = { t.m8, t.m9, t.m10};
// 
// 	float speed = Vector3Length(v->vel);
// 
//  	float dot = Vector3DotProduct(forward, v->vel);
//  	int direction = (signbit(dot) ? -1 : 1);
// 
// 		/* UPDATE ACCEL */
// 	Vector3 forceTraction	= Vector3Scale(forward, inputThrottle*driveForce);
// 	Vector3 forceBrake	= Vector3Scale(forward, -inputBrake*direction*car.brakeForce);
// 	Vector3 forceDrag	= Vector3Scale(v->vel, -car.drag * speed);
// 	Vector3 forceRR		= Vector3Scale(v->vel, -car.rr);
// 
// 	Vector3 force = { 0 };
// 	force = Vector3Add(force, forceTraction);
// 	force = Vector3Add(force, forceBrake);
// 	force = Vector3Add(force, forceDrag); // TODO does it make sense to add drag here?
// 	force = Vector3Add(force, forceRR);
// 
// 	Vector3 forceDirection = Vector3Normalize(force);
// 	float forceMagnitude = Vector3Length(force);
// 	float forceCapped = fmin(forceMagnitude, maxForce);
// 
// 	force = Vector3Scale(forceDirection, forceCapped);
// 
// 	Vector3 accel = Vector3Scale(force, 1.0f/car.mass); // FORCE / MASS
// 
// 		/* UPDATE VELOCITY */
// 	v->vel = Vector3Add(v->vel, Vector3Scale(accel, deltaTime));
// 
// 		/* UPDATE POSITION */
// 	Vector3 translation = Vector3Scale(v->vel, deltaTime);
// 	v->origin.m12 += translation.x;
// 	v->origin.m13 += translation.y;
// 	v->origin.m14 += translation.z;
// 
// 	return;
// }

Camera VehicleCameraUpdate(Vehicle* v)
{
	Camera c = {
		.position = (Vector3){ 0.0f, 4.0f, -8.0f },
		.target = (Vector3){ 0.0f, 1.0f, 0.0f },
		.up = (Vector3){ 0.0, 1.0f, 0.0f },
		.fovy = 45.0f,
		.projection = CAMERA_PERSPECTIVE
	};

	Matrix t = v->origin;
	Vector3 position = { t.m12, t.m13, t.m14};
	Quaternion rotation = QuaternionFromMatrix(t);

	c.target = Vector3Add(c.target, position);
	c.position = Vector3RotateByQuaternion(c.position, rotation);
	c.position = Vector3Add(c.position, position);

	return c;
}
#endif /* VEHICLE_H */

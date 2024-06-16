#include "raylib.h"
#include "raymath.h"

#ifndef GAMEPAD_ID
#define GAMEPAD_ID 0
#endif

#ifndef GIZMO_H
#define GIZMO_H

// Eventually these are to be turned into vehicle stats. 
// This just makes it easier to compile because i don't
// need to worry about the unused variables.
#define ACCEL 2.0f //2 m/s**2
#define DRAG_COEFFICIENT 0.02f 
#define STEER_SPEED (PI/2.0f) //TODO for now, steer is a fixed speed. later i want it implemented with aerodynamics
#define RUDDER_EFFECT 0.10f //think of this as ratio of the rudder's affect on the ships steer
#define MAX_RUDDER_ANGLE (PI/4.0f) //45 degrees


typedef struct Gizmo {
	Model* model;
	Vector3 position;
	Vector3 velocity;
	float yaw;
} Gizmo;

void updateGizmo(Gizmo* g, float cameraYaw) {

	(void)cameraYaw;//TODO unused variable, i plan on using it later for slip mode.

	//LOCAL VARIABLE DECLARTIONS
	float deltaTime = GetFrameTime();
	//float velMag = Vector3Length(g->velocity);
	float xAccel = 0;
	float zAccel = 0;
	float xAxis = 0;
	//float yAxis = 0;
	float rTrigger = 0;

	//OBTAIN INPUTS
	xAxis = GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X);
	//yAxis = GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_Y);
	rTrigger = (1+(GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_RIGHT_TRIGGER)))/2;//some quick maff to turn it into [0..1] range

	//CONVERT INPUTS INTO FORCES
	if(rTrigger > EPSILON) {//apply thruster force
		xAccel = rTrigger*sinf(g->yaw)*ACCEL;//TODO check to make sure that we don't need deltaTime here.
		zAccel = rTrigger*cosf(g->yaw)*ACCEL;
	}

	if((xAxis > EPSILON) || (xAxis < -EPSILON)) {//apply steering
		g->yaw -= deltaTime*xAxis*STEER_SPEED;
	}

	g->yaw = Wrap(g->yaw, 0, PI*2); //keep rotations within [0..TAU]

	//APPLY FORCES
	//theoretically this is thruster+aerodynamics/drag
	g->velocity.x += xAccel*deltaTime;
	g->velocity.z += zAccel*deltaTime;

	g->position.x += g->velocity.x;
	g->position.z += g->velocity.z;

	g->velocity.x *= 1-DRAG_COEFFICIENT;//apply drag after acceleration is applied
	g->velocity.z *= 1-DRAG_COEFFICIENT;

	

}

#endif //GIZMO_H

#include "raylib.h"
#include "raymath.h"

#ifndef GAMEPAD_ID
#define GAMEPAD_ID 0
#endif

#ifndef GIZMO_H
#define GIZMO_H
//eventually be turned into struct members
#define THRUST 2000.0f //in newtons
#define WEIGHT 1000.0f //in kilograms
#define DRAG 0.02f //so the thing slow down
#define STEER_SPEED (PI/2.0f)
#define GRIP 1.0f //coefficient of friction

typedef struct Gizmo {
	Model* model;
	Vector3 position;
	Vector3 velocity;
	float yaw;//[-PI,PI] range
} Gizmo;

void updateGizmo(Gizmo* g) {

	//LOCAL VARIABLE DECLARTIONS
	float deltaTime = GetFrameTime();
	float xAxis = GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X);
	float rTrigger = (1.0f+(GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_RIGHT_TRIGGER)))/2.0f;//some quick maff to turn it into [0..1] range
	float det;//these have to do with steering
	float sine;//TODO should yaw be replaced with a unit vector? discuss.
	float cosine;

	//VEHICLE ROTATION
	g->yaw += -1.0f*deltaTime*xAxis*STEER_SPEED;
	g->yaw = Wrap(g->yaw, -PI, PI);

	sine = sinf(g->yaw); //TODO declare locals here instead?
	cosine = cosf(g->yaw);

	//STEERING PHYSICS
	det = (g->velocity.x * cosine) - (g->velocity.z * sine);//???
	g->velocity.x += det * -cosine;
	g->velocity.z += det * sine;

	//APPLY THRUSTER FORCE
	if(rTrigger > EPSILON) {
		g->velocity.x += deltaTime*rTrigger*sine*(THRUST/WEIGHT);
		g->velocity.z += deltaTime*rTrigger*cosine*(THRUST/WEIGHT);
	}



	//APPLY FORCES
	//theoretically this is thruster+aerodynamics/drag
	g->position.x += deltaTime*g->velocity.x;
	g->position.z += deltaTime*g->velocity.z;

	g->velocity.x = g->velocity.x * (1-deltaTime*DRAG);//apply drag after acceleration is applied
	g->velocity.z = g->velocity.z * (1-deltaTime*DRAG);

	

}

#endif //GIZMO_H

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
	Vector2 steerVector = (Vector2){sinf(g->yaw), cosf(g->yaw)};
	Vector2 gripVector = (Vector2){sinf(g->yaw - PI/2), cosf(g->yaw - PI/2)}; //the lateral grip unit vector :v
	float det = 0.0f;


	//VEHICLE ROTATION
	if( !(FloatEquals(xAxis, 0.0f)) ) {//TODO if statement might be pointless
		g->yaw += -1.0f*deltaTime*xAxis*STEER_SPEED;
		g->yaw = Wrap(g->yaw, -PI, PI);
	}

	//STEERING PHYSICS
	//first we take the determinant of Velocity and steerVector
	det = (g->velocity.x * steerVector.y) - (g->velocity.z * steerVector.x);
	DrawText(TextFormat("Determinant: %f", det), 20, 65, 10, RED);

	//scale gripVector by this determinant
	gripVector = Vector2Scale(gripVector, det);//TODO more deltaTime confusion, does steering only apply a fraction of the velocity?
	//add to velocity!
	g->velocity = Vector3Add(g->velocity, (Vector3){gripVector.x, 0.0f, gripVector.y});



	//APPLY THRUSTER FORCE
	if(rTrigger > EPSILON) {
		g->velocity.x += deltaTime*rTrigger*sinf(g->yaw)*sinf(g->yaw)*(THRUST/WEIGHT);
		g->velocity.z += deltaTime*rTrigger*cosf(g->yaw)*cosf(g->yaw)*(THRUST/WEIGHT);
	}



	//APPLY FORCES
	//theoretically this is thruster+aerodynamics/drag
	g->position.x += deltaTime*g->velocity.x;
	g->position.z += deltaTime*g->velocity.z;

	g->velocity.x = g->velocity.x * (1-deltaTime*DRAG);//apply drag after acceleration is applied
	g->velocity.z = g->velocity.z * (1-deltaTime*DRAG);

	

}

#endif //GIZMO_H

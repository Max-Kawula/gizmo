#include "raylib.h"
#include "raymath.h"

#ifndef GAMEPAD_ID
#define GAMEPAD_ID 0
#endif

#ifndef GIZMO_H
#define GIZMO_H

//eventually be turned into struct members
#define THRUST 6000.0f //newtons
#define WEIGHT 3000.0f //kg
#define DRAG_COEFFICIENT 0.90f //i have no idea
#define MAX_RUDDER_SPEED (PI/6.0f) //30 degrees per second (at a certain speed)
#define MAX_DIVING_ANGLE (PI/6.0f)


typedef struct Gizmo {
	Model* model;
	Vector3 position;
	Vector3 velocity;
	Vector3 rotation;
	Vector3 angularVel;//this is a really dumb idea
	BoundingBox volume; //this is a dumb idea
} Gizmo;

Gizmo initGizmo(Model* model) {

	/* 
	 * loading a model is confusing the shit out of me,
	 * need instance it in the main code
	*/

	Gizmo g = { 0 };

	g.model = model;//pointer assinged to pointer
 	g.volume = GetModelBoundingBox(*model);//dereference model

	return g;

}

void updateGizmo(Gizmo* g, float camYaw) {

	/*
	 * this is my most unreadable code yet
	 * im basically grabbing a ton of inputs and transforming them at the same time
	 * im also doing all local variable declarations at the start of the block
	 * because the internet told me to do things like this
	 */
	float deltaTime = GetFrameTime();
	Vector2 inputDir;
	inputDir.x = GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_X);
	inputDir.y = GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_Y);
	float stickMag = Vector2Length(inputDir);
	Vector2 intendedDir = Vector2Rotate( Vector2Normalize(inputDir), camYaw );
	float rTrigger = (1.0f+(GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_RIGHT_TRIGGER)))/2.0f;
	float lTrigger = (1.0f+(GetGamepadAxisMovement(GAMEPAD_ID, GAMEPAD_AXIS_LEFT_TRIGGER)))/2.0f;
	float intendedPitch = (rTrigger - lTrigger) * sinf(PI/6.0f);
	Vector2 gizDir = { sinf(g->rotation.y), cosf(g->rotation.y) };
	float dot = Vector2DotProduct(gizDir, intendedDir);
	float det = (gizDir.x * intendedDir.y) - (gizDir.y * intendedDir.x);

	//stickMag determines percentage of thrust
	//the fucking stick is able to produce values outside of 1.0f
	//all thanks to it not being a circle
	if(stickMag > 1.0f) {
		stickMag = 1.0f;
	}

	//dot will represent how much steering is needed to go towards intendedDir
	dot = (dot - 1)/-2; //put dot into a [0.0,1.0] range





	g->position.x += deltaTime*g->velocity.x;
	g->position.z += deltaTime*g->velocity.z;

	g->velocity.x = //drag is something like 0.5f * DRAG * velocity^2
	g->velocity.z = //this produces a force, which then accelerates the mass

}

#endif //GIZMO_H

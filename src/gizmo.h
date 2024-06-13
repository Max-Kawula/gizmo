#include "raylib.h"
#include "raymath.h"

#ifndef GAMEPAD_ID
#define GAMEPAD_ID 0
#endif

#ifndef GIZMO_H
#define GIZMO_H

const float gizmoSpeed = 3.0f;

typedef struct Gizmo {
	Model* model;
	Vector3 position;
	float yaw;
} Gizmo;

void updateGizmo(Gizmo* g, float cameraYaw) {

	(void)cameraYaw;//im supressing an unused parameter

	float xAxis = GetGamepadAxisMovement(GAMEPAD_ID, 0);
	float zAxis = GetGamepadAxisMovement(GAMEPAD_ID, 1);

	g->position.x -= gizmoSpeed*GetFrameTime()*xAxis;
	g->position.z -= gizmoSpeed*GetFrameTime()*zAxis;

}

#endif //GIZMO_H

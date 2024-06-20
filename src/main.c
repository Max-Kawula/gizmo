#include <stdio.h>
//#include <stdlib.h>//using malloc
#include "raylib.h"
#include "raymath.h"
#include "gamecamera.h"
#include "gizmo.h"

#ifndef GAMEPAD_ID
#define GAMEPAD_ID 0
#endif


#define VEC3UP (Vector3){ 0.0f, 1.0f, 0.0f }

/////////////
// PROGRAM //
/////////////
int main(void) {

	const int screenWidth = 800;
	const int screenHeight = 600;

	InitWindow(screenWidth, screenHeight, "raylib - gizmo");

	Model gizmoModel = LoadModel("assets/models/gizmoid.obj");

	Gizmo gizmo;
	gizmo.model = &gizmoModel;
	gizmo.position = Vector3Zero();
	gizmo.yaw = 0.0f;

	GameCamera gameCamera = { 0 };
	gameCamera.distance = 10.0f;

	SetTargetFPS(60);

	
	//GAME LOOP
	while (!WindowShouldClose()) {//UPDATE
		
		updateGizmo( &gizmo );
		updateGameCamera( &gameCamera, gizmo.position );

		BeginDrawing();//DRAW

			ClearBackground(RAYWHITE);

			BeginMode3D( GameCameraToCamera3D(gameCamera) );//where the magic happens

				DrawGrid(100,1.0f);
				DrawSphere( (Vector3){ 5.0f, 0.0f, 0.0f }, 0.3f, PINK );//x
				DrawSphere( (Vector3){ 0.0f, 0.0f, 5.0f }, 0.3f, SKYBLUE );//z

				DrawModelEx(*gizmo.model, gizmo.position, VEC3UP, gizmo.yaw*RAD2DEG, Vector3One(), WHITE);
				//i have a sneaking suspicion that the angle needs to be provided in degrees. if true, highly fucking stupid.

			EndMode3D();
			
			//HUD
			DrawText(TextFormat("xVel: %f", gizmo.velocity.x), 20, 20, 10, RED);
			DrawText(TextFormat("zVel: %f", gizmo.velocity.z), 20, 35, 10, BLUE);
			DrawText(TextFormat("yaw: %f Radians", gizmo.yaw), 20, 50, 10, GREEN);

		EndDrawing();

	}
	
	CloseWindow();

	return 0; 
}

//DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint);

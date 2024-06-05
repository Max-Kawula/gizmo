#include <stdio.h>
#include <stdlib.h>//using malloc
#include "raylib.h"
#include "rlgl.h"

#define RAYLIB_NEW_RLGL

typedef struct gizmo {
	Model model;
	Vector3 pos;
	float angle;
	Vector3 vel;
} gizmo;

void updateGizmo(gizmo* g);


/////////////
// PROGRAM //
/////////////
int main(void) {

	gizmo giz;
	giz.pos = (Vector3){0.0f,0.0f,0.0f};
	giz.angle = 0.0f;
	giz.vel = (Vector3){0.0f,0.0f,0.0f}; 
	giz.model = LoadModel("../assets/models/gizmoid.obj");
	*giz.model.materials = LoadMaterialDefault(); //TODO give the dude a mat
	
	const int screenWidth = 640;
	const int screenHeight = 480;

	InitWindow(screenWidth, screenHeight, "raylib - gizmo");

    	Camera3D camera = { 0 };
    	camera.position = (Vector3){ -10.0f, 15.0f, -10.0f };
    	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    	camera.fovy = 45.0f;
    	camera.projection = CAMERA_PERSPECTIVE;

	DisableCursor();
	SetTargetFPS(60);

	//GAME LOOP
	while (!WindowShouldClose()) {//UPDATE
		UpdateCamera(&camera, CAMERA_ORBITAL);

		BeginDrawing();
			ClearBackground(RAYWHITE);

			BeginMode3D(camera);//DRAW

				DrawGrid(10,1.0f);
				DrawModelEx( giz.model, giz.pos, (Vector3){0.0f,1.0f,0.0}, giz.angle, (Vector3){1.0f,1.0f,1.0f}, WHITE );

			EndMode3D();
		EndDrawing();

	}
	
	CloseWindow();

	return 0; 
}

//DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle, Vector3 scale, Color tint);

	/////////////////////
	// RAYLIB TEMPLATE //
	/////////////////////

#include "raylib.h"
#include "raymath.h"

#include "vehicle.h"

#define GLSL_VERSION 330

int main(void) {
	
	const int screenWidth = 800;
	const int screenHeight = 600;

	// TODO Change my name!
	InitWindow(screenWidth, screenHeight, "raylib - vehicles");
	/* LOAD THINGS */
	
	Camera camera = {
		{30.0f, 30.0f, 30.0f},	//position
		{0.0f, 0.0f, 0.0f},	//target
		{0.0f, 1.0f, 0.0f},	//up
		20.0f,			//fovy
		CAMERA_ORTHOGRAPHIC	//projection
	};
	
	SetTargetFPS(60);


	/* LOAD ASSETS */
	Model car = LoadModel("assets/models/86.obj");

	Shader carShader = LoadShader(	TextFormat("assets/shaders/vehicle.fs"),
					TextFormat("assets/shaders/vehicle.vs"));
	
	Texture2D carTex = LoadTexture("assets/textures/86.png");

	car.materials[0].shader = carShader;
	car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = carTex;

	Vehicle veh = {
		.origin = MatrixIdentity(),
		.vel = Vector3Zero(),
		.angularVel = QuaternionIdentity(),
		.input = { 0 }
	};

	/////////////////////
	// GAME LOOP BEGIN //
	/////////////////////
	
	while(!WindowShouldClose()) {

		VehicleMonoUpdate(&veh);
		car.transform = veh.origin;

		BeginDrawing();
			/* DRAW HERE */
			ClearBackground((Color){50,50,80,255});

			BeginMode3D(camera);
				DrawGrid(50, 2);
				DrawModel(car, Vector3Zero(), 1.0f, WHITE);
			EndMode3D();

		EndDrawing();

	}
	UnloadModel(car);
	UnloadTexture(carTex);
	UnloadShader(carShader);
	return 0;
}

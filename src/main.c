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
		{0.0f, 5.0f, 10.0f},	//position
		{0.0f, 0.0f, 0.0f},	//target
		{0.0f, 1.0f, 0.0f},	//up
		60.0f,			//fovy
		CAMERA_PERSPECTIVE	//projection
	};
	
	SetTargetFPS(60);


	/* LOAD ASSETS */
	Model car = LoadModel("assets/models/86.obj");

	Shader carShader = LoadShader(	TextFormat("assets/shaders/vehicle.fs"),
					TextFormat("assets/shaders/vehicle.vs"));
	
	Texture2D carTex = LoadTexture("assets/textures/86.png");

	car.materials[0].shader = carShader;
	car.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = carTex;


	/* INIT VEHICLE */
	Vehicle veh = { 
		.transform = MatrixIdentity(),
		.vel = Vector3Zero(),
	};

	/////////////////////
	// GAME LOOP BEGIN //
	/////////////////////
	
	while(!WindowShouldClose()) {
		/* UPDATE LOGIC */	
		// UpdateCamera(&camera, CAMERA_ORBITAL);

		VehicleControl(&veh);
		car.transform = veh.transform; // match model to object transform
		camera = VehicleCameraUpdate(&veh);


		BeginDrawing();
			/* DRAW HERE */
			ClearBackground((Color){40,40,40,255});

			BeginMode3D(camera);
				DrawGrid(50, 1);
				DrawModel(car, Vector3Zero(), 1.0f, WHITE);
			EndMode3D();

		EndDrawing();

	}
	UnloadModel(car);
	UnloadTexture(carTex);
	UnloadShader(carShader);
	return 0;
}

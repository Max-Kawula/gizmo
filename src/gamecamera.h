//GameCamera is an object that will facilitate Camera
//movement around the active subject and gamepad inputs.

#include "raylib.h"
#include "raymath.h"

#ifndef GAMEPAD_ID
#define GAMEPAD_ID 0
#endif

#ifndef GAMECAMERA_H
#define GAMECAMERA_H

const float cameraSpeed = PI; //180d per second
const float pitchMax = 85.0f*DEG2RAD;
const float pitchMin = 5.0f*DEG2RAD;
const float cameraFov = 45.0f;

//GameCamera is the ONLY way to interact with the camera
//rather than having two objects doing the same thing,
//GameCamera will be converted when BeginMode3D() is called.
typedef struct GameCamera {
	Vector3 origin;
	float yaw; //yaw and pitch will produce a 3D unit vector
	float pitch;
	float distance; //scaled by distance
} GameCamera;


//this needs to be pass by reference because the camera data gets modified
void updateGameCamera(GameCamera* gCam, Vector3 target) {//FIXME there seems to be a problem with updating the camera origin
	gCam->origin = target;
	gCam->yaw -= cameraSpeed*GetFrameTime()*GetGamepadAxisMovement(GAMEPAD_ID, 2);//right joystick x
	gCam->pitch -= cameraSpeed*GetFrameTime()*GetGamepadAxisMovement(GAMEPAD_ID, 3);//right joystick y
	
	gCam->yaw = Wrap(gCam->yaw, 0, PI*2);
	gCam->pitch = Clamp(gCam->pitch, pitchMin, pitchMax); 
}

//convert GameCamera to Camera so it can be passed to BeginMode3D();
//this is pass by value because this should have zero effect on GameCamera
Camera GameCameraToCamera3D(GameCamera gCam) {

	Camera3D camera = { 0 }; //locally stored Camera3D
	
	//camera.position = gCam.origin + (Vector3){ 0.0f, 0.0f, 1.0f };
	camera.target = gCam.origin;
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = cameraFov;
	camera.projection = CAMERA_PERSPECTIVE;
	
	float y = sinf(gCam.pitch);
	float hScale = cosf(gCam.pitch);
	//hScale = hScale*hScale;//cosine squared!
				
	float z = hScale*cosf(gCam.yaw);//FIXME something is inheritly wrong with adding sin and cos together, maybe square results?
	float x = hScale*sinf(gCam.yaw);

	camera.position = Vector3Scale( (Vector3){ x, y, z }, gCam.distance );

	return camera;
}




#endif //GAMECAMERA_H

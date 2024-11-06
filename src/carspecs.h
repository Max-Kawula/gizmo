#ifndef CARSPECS_H
#define CARSPECS_H

typedef struct CarSpec {

	// transmission spec
	float engineTorque; // (Nm)
	float gearRatio[7]; // gear 0 is reverse!!!
	float diffRatio; // a.k.a. final drive ratio
	float wheelRadius; // (m)

	//constants
	float brakeForce; // (N)
	float drag; // constant
	float rr; // constant
	float mass; // (kg)
	float mu;

} CarSpec;

// https://www.aeu86.org/technical/ae86-specifications/
const CarSpec ae86 = {
	.engineTorque = 145.0f,
	.gearRatio[0] = 3.480f,
	.gearRatio[1] = 3.587f,
	.gearRatio[2] = 2.022f,
	.gearRatio[3] = 1.384f,
	.gearRatio[4] = 1.000f,
	.gearRatio[5] = 0.861f,
	.gearRatio[6] = 0.000f, //not sure if i need to initialize it?
	.diffRatio = 4.30f, // confusion on source "4:30:1", although that seems like a typo?
	.wheelRadius = 0.30f,
	.brakeForce = 20000.0f,
	.drag = 0.35f,
	.rr = 10.5f,
	.mass = 970.0f,
	.mu = 1.0f
};
#endif /* CARSPECS_H */


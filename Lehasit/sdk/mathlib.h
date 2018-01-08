#pragma once

#include <cmath>

#include "Vector.h"
#include "QAngle.h"

#define M_PI 3.14159265358979323846f
#define DEG2RAD(x) ((float)(x) * (M_PI / 180.f))
#define RAD2DEG(x) ((float)(x) * (180.f / M_PI))

using matrix3x4_t = float[3][4];

void TransformVector(const Vector in1, const matrix3x4_t& in2, Vector& out);

float NormalizeAngle(float angle);
float NormalizeAnglePositive(float angle);

constexpr float PI = 3.14159265358979323846f;

inline float DegToRad(float f)
{
	return f * (PI / 180.f);
}

inline float RadToDeg(float f)
{
	return f * (180.f / PI);
}

/*void inline SinCos(float radians, float& sine, float& cosine) {
	sine = sinf(radians);
	cosine = cosf(radians);
}

void AngleVectors(const QAngle& angles, Vector& forward) {
	float sp, sy, cp, cy;

	SinCos(DEG2RAD(angles[YAW]), sy, cy);
	SinCos(DEG2RAD(angles[PITCH]), sp, cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}*/
/*
void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up) {
	float sr, sp, sy, cr, cp, cy;

	SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
	SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
	SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

	if (forward) {
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up) {
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}*/

/*void VectorAngles(const Vector& forward, QAngle& angles) {
	float tmp, yaw, pitch;

	if (forward.x == 0.f && forward.y == 0.f) {
		yaw = 0.f;
		if (forward.z > 0.f) {
			pitch = 270.f;
		} else {
			pitch = 90.f;
		}
	} else {
		yaw = RAD2DEG(atan2f(forward.y, forward.x));
		if (yaw < 0.f)
			yaw += 360.f;

		tmp = forward.length2D();
		pitch = RAD2DEG(atan2f(-forward.z, tmp));

		if (pitch < 0.f)
			pitch += 360.f;
	}

	angles[PITCH] = pitch;
	angles[YAW] = yaw;
	angles[ROLL] = 0.f;
}*/
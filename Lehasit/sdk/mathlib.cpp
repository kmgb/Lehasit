#include "stdafx.h"
#include "mathlib.h"

float DotProduct(const Vector v1, const float* v2)
{
	return (v1.x * v2[0]) + (v1.y * v2[1]) + (v1.z * v2[2]);
}

void TransformVector(const Vector in1, const matrix3x4_t& in2, Vector& out)
{
	out.x = DotProduct(in1, in2[0]) + in2[0][3];
	out.y = DotProduct(in1, in2[1]) + in2[1][3];
	out.z = DotProduct(in1, in2[2]) + in2[2][3];
}

float NormalizeAngle(float angle)
{
	angle = fmod(angle, 360.f);

	if (angle > 180.f)
		angle -= 360.f;

	else if (angle < -180.f)
		angle += 360.f;

	return angle;
}

float NormalizeAnglePositive(float angle)
{
	angle = fmod(angle, 360.f);

	if (angle < 0.f)
	{
		angle += 360.f;
	}

	return angle;
}

void VectorAngles(const Vector& forward, QAngle& angles)
{
	float tmp, yaw, pitch;

	if (forward.x == 0.f && forward.y == 0.f)
	{
		yaw = 0.f;
		if (forward.z > 0.f)
		{
			pitch = 270.f;
		}
		else
		{
			pitch = 90.f;
		}
	}
	else
	{
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
}
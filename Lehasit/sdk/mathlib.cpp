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
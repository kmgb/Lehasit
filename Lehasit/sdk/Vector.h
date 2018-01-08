#pragma once

// Credits: Valve
// Stolen from gir, who stole it from the sdk

class Vector {
public:
	float x, y, z;
	Vector(void);
	Vector(float X, float Y, float Z);
	void init(float ix = 0.f, float iy = 0.f, float iz = 0.f);
	inline void zero();
	/*float*/void normalize();
	__forceinline float	DistToSqr(const Vector &vOther) const;
	float dot(const Vector& vOther) const;
	float length() const;
	float length2D(void) const;
	float length2DSqr(void) const;

	float operator[](int i) const;
	float& operator[](int i);
	bool operator==(const Vector& v) const;
	bool operator!=(const Vector& v) const;
	bool isZero(float tolerance = 0.01f) const {
		return (x > -tolerance && x < tolerance
			&& y > -tolerance && y < tolerance
			&& z > -tolerance && z < tolerance);
	}

	__forceinline Vector& operator+=(const Vector &v);
	__forceinline Vector& operator-=(const Vector &v);
	__forceinline Vector& operator*=(const Vector &v);
	__forceinline Vector& operator*=(float s);
	__forceinline Vector& operator/=(const Vector &v);
	__forceinline Vector& operator/=(float s);
	__forceinline Vector& operator+=(float fl);
	__forceinline Vector& operator-=(float fl);

	Vector& operator=(const Vector &vOther);
	Vector	operator+(const Vector& v) const;
	Vector	operator-(const Vector& v) const;
	Vector	operator*(const Vector& v) const;
	Vector	operator/(const Vector& v) const;
	Vector	operator*(float fl) const;
	Vector	operator/(float fl) const;
};
//===============================================
inline void Vector::init(float ix, float iy, float iz) {
	x = ix; y = iy; z = iz;
}
//===============================================
inline Vector::Vector(float X, float Y, float Z) {
	x = X; y = Y; z = Z;
}
//===============================================
inline Vector::Vector(void) {
}
//===============================================
inline void Vector::zero() {
	x = y = z = 0.f;
}
//===============================================
inline void VectorClear(Vector& a) {
	a.x = a.y = a.z = 0.f;
}
//===============================================
inline Vector& Vector::operator=(const Vector &vOther) {
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}
//===============================================
inline float& Vector::operator[](int i) {
	return ((float*)this)[i];
}
//===============================================
inline float Vector::operator[](int i) const {
	return ((float*)this)[i];
}
//===============================================
inline bool Vector::operator==(const Vector& src) const {
	return (src.x == x) && (src.y == y) && (src.z == z);
}
//===============================================
inline bool Vector::operator!=(const Vector& src) const {
	return (src.x != x) || (src.y != y) || (src.z != z);
}
//===============================================
__forceinline void VectorCopy(const Vector& src, Vector& dst) {
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
//===============================================
__forceinline  Vector& Vector::operator+=(const Vector& v) {
	x += v.x; y += v.y; z += v.z;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator-=(const Vector& v) {
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator*=(float fl) {
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator*=(const Vector& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}
//===============================================
__forceinline Vector&	Vector::operator+=(float fl) {
	x += fl;
	y += fl;
	z += fl;
	return *this;
}
//===============================================
__forceinline Vector&	Vector::operator-=(float fl) {
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator/=(float fl) {
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}
//===============================================
__forceinline  Vector& Vector::operator/=(const Vector& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}
//===============================================
inline float Vector::length(void) const {
	float root = 0.f;

	float sqsr = x*x + y*y + z*z;

	__asm
	{
		sqrtss xmm0, sqsr
		movss root, xmm0
	}

	return root;
}
//===============================================
inline float Vector::length2D(void) const {
	float root = 0.f;

	float sqst = x*x + y*y;

	__asm {
		sqrtss xmm0, sqst
		movss root, xmm0
	}

	return root;
}
//===============================================
inline float Vector::length2DSqr(void) const {
	return (x*x + y*y);
}
//===============================================
inline Vector CrossProduct(const Vector& a, const Vector& b) {
	return Vector(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
//===============================================
inline void Vector::normalize() {
	Vector& v = *this;

	float iradius = 1.f / (this->length() + 1.192092896e-07F); //FLT_EPSILON

	v.x *= iradius;
	v.y *= iradius;
	v.z *= iradius;
}
//===============================================
inline Vector Vector::operator+(const Vector& v) const {
	Vector res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}
//===============================================
inline Vector Vector::operator-(const Vector& v) const {
	Vector res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}
//===============================================
inline Vector Vector::operator*(float fl) const {
	Vector res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}
//===============================================
inline Vector Vector::operator*(const Vector& v) const {
	Vector res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}
//===============================================
inline Vector Vector::operator/(float fl) const {
	Vector res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}
//===============================================
inline Vector Vector::operator/(const Vector& v) const {
	Vector res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}
inline float Vector::dot(const Vector& vOther) const {
	const Vector& a = *this;

	return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
}
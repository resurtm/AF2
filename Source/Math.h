#pragma once

#if !defined PI
#define PI ((float)3.141592654f)
#endif	//	#if !defined PI

inline float DegToRad(const float f) { return f * PI / 180.0f; }
inline float RadToDeg(const float f) { return f * 180.0f / PI; }

class CVector2 {
public:
	inline CVector2(void): x(0.0f), y(0.0f) {}
	inline CVector2(const float v): x(v), y(v) {}
	inline CVector2(const float _x, const float _y): x(_x), y(_y) {}
	inline ~CVector2(void) { x = y = 0.0f; }

	inline const float &operator [](const int i) const { return (&x)[i]; }
	inline float &operator [](const int i) { return (&x)[i]; }
	inline operator const float *(void) const { return &x; }
	inline operator float *(void) { return &x; }

	inline CVector2 operator -(void) const { return CVector2(-x, -y); }
	inline CVector2 operator !(void) const { return CVector2(-x, -y); }
	inline bool operator ==(const CVector2 &v) const { return x == v.x && y == v.y; }
	inline bool operator !=(const CVector2 &v) const { return x != v.x && y != v.y; }
	inline bool operator <(const CVector2 &v) const { return x < v.x && y < v.y; }
	inline bool operator >(const CVector2 &v) const { return x > v.x && y > v.y; }
	inline bool operator <=(const CVector2 &v) const { return x <= v.x && y <= v.y; }
	inline bool operator >=(const CVector2 &v) const { return x >= v.x && y >= v.y; }

	inline CVector2 operator +(const CVector2 &v) const { return CVector2(x + v.x, y + v.y); }
	inline CVector2 operator -(const CVector2 &v) const { return CVector2(x - v.x, y - v.y); }
	inline CVector2 operator *(const CVector2 &v) const { return CVector2(x * v.x, y * v.y); }
	inline CVector2 operator /(const CVector2 &v) const { return CVector2(x / v.x, y / v.y); }
	inline CVector2 operator +(const float f) const { return CVector2(x + f, y + f); }
	inline CVector2 operator -(const float f) const { return CVector2(x - f, y - f); }
	inline CVector2 operator *(const float f) const { return CVector2(x * f, y * f); }
	inline CVector2 operator /(const float f) const { return CVector2(x / f, y / f); }

	inline CVector2 &operator +=(const CVector2 &v) { *this = *this + v; return *this; }
	inline CVector2 &operator -=(const CVector2 &v) { *this = *this - v; return *this; }
	inline CVector2 &operator *=(const CVector2 &v) { *this = *this * v; return *this; }
	inline CVector2 &operator /=(const CVector2 &v) { *this = *this / v; return *this; }
	inline CVector2 &operator +=(const float f) { *this = *this + f; return *this; }
	inline CVector2 &operator -=(const float f) { *this = *this - f; return *this; }
	inline CVector2 &operator *=(const float f) { *this = *this * f; return *this; }
	inline CVector2 &operator /=(const float f) { *this = *this / f; return *this; }

	inline float operator |(const CVector2 &v) const { return x * v.x + y * v.y; }

	float x, y;

protected:
private:
};	//	class CVector2;

class CVector3 {
public:
	inline CVector3(void): x(0.0f), y(0.0f), z(0.0f) {}
	inline CVector3(const float v): x(v), y(v), z(v) {}
	inline CVector3(const float _x, const float _y, const float _z): x(_x), y(_y), z(_z) {}
	inline ~CVector3(void) { x = y = z = 0.0f; }

	inline const float &operator [](const int i) const { return (&x)[i]; }
	inline float &operator [](const int i) { return (&x)[i]; }
	inline operator const float *(void) const { return &x; }
	inline operator float *(void) { return &x; }

	inline CVector3 operator -(void) const { return CVector3(-x, -y, -z); }
	inline CVector3 operator !(void) const { return CVector3(-x, -y, -z); }
	inline bool operator ==(const CVector3 &v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool operator !=(const CVector3 &v) const { return x != v.x && y != v.y && z != v.z; }
	inline bool operator <(const CVector3 &v) const { return x < v.x && y < v.y && z < v.z; }
	inline bool operator >(const CVector3 &v) const { return x > v.x && y > v.y && z > v.z; }
	inline bool operator <=(const CVector3 &v) const { return x <= v.x && y <= v.y && z <= v.z; }
	inline bool operator >=(const CVector3 &v) const { return x >= v.x && y >= v.y && z >= v.z; }

	inline CVector3 operator +(const CVector3 &v) const { return CVector3(x + v.x, y + v.y, z + v.z); }
	inline CVector3 operator -(const CVector3 &v) const { return CVector3(x - v.x, y - v.y, z - v.z); }
	inline CVector3 operator *(const CVector3 &v) const { return CVector3(x * v.x, y * v.y, z * v.z); }
	inline CVector3 operator /(const CVector3 &v) const { return CVector3(x / v.x, y / v.y, z / v.z); }
	inline CVector3 operator +(const float f) const { return CVector3(x + f, y + f, z + f); }
	inline CVector3 operator -(const float f) const { return CVector3(x - f, y - f, z - f); }
	inline CVector3 operator *(const float f) const { return CVector3(x * f, y * f, z * f); }
	inline CVector3 operator /(const float f) const { return CVector3(x / f, y / f, z / f); }

	inline CVector3 &operator +=(const CVector3 &v) { *this = *this + v; return *this; }
	inline CVector3 &operator -=(const CVector3 &v) { *this = *this - v; return *this; }
	inline CVector3 &operator *=(const CVector3 &v) { *this = *this * v; return *this; }
	inline CVector3 &operator /=(const CVector3 &v) { *this = *this / v; return *this; }
	inline CVector3 &operator +=(const float f) { *this = *this + f; return *this; }
	inline CVector3 &operator -=(const float f) { *this = *this - f; return *this; }
	inline CVector3 &operator *=(const float f) { *this = *this * f; return *this; }
	inline CVector3 &operator /=(const float f) { *this = *this / f; return *this; }

	inline float operator |(const CVector3 &v) const { return x * v.x + y * v.y + z * v.z; }
	inline CVector3 operator ^(const CVector3 &v) const { return CVector3(v.y * z - v.z * y, v.z * x - v.x * z, v.x * y - v.y * x); }

	float x, y, z;

protected:
private:
};	//	class CVector3;

class CMatrix {
public:
	inline CMatrix(void) { memset(m, 0, 64); m[0] = m[5] = m[10] = m[15] = 1.0f; }
	inline CMatrix(const float *v) { memcpy(m, v, 64); }
	inline ~CMatrix(void) { memset(m, 0, 64); }

	inline const float &operator [](const int i) const { return m[i]; }
	inline float &operator [](const int i) { return m[i]; }
	inline operator const float *(void) const { return m; }
	inline operator float *(void) { return m; }

	inline CMatrix operator +(const CMatrix &v) const { float _m[16]; for (unsigned int i=0; i<16; i++) _m[i] = m[i] + v.m[i]; return CMatrix(_m); }
	inline CMatrix operator -(const CMatrix &v) const { float _m[16]; for (unsigned int i=0; i<16; i++) _m[i] = m[i] - v.m[i]; return CMatrix(_m); }
	inline CMatrix operator *(const CMatrix &v) const {
		CMatrix _m;
		_m[ 0] = (m[ 0]*v[ 0]) + (m[ 4]*v[ 1]) + (m[ 8]*v[ 2]) + (m[12]*v[ 3]);
		_m[ 1] = (m[ 1]*v[ 0]) + (m[ 5]*v[ 1]) + (m[ 9]*v[ 2]) + (m[13]*v[ 3]);
		_m[ 2] = (m[ 2]*v[ 0]) + (m[ 6]*v[ 1]) + (m[10]*v[ 2]) + (m[14]*v[ 3]);
		_m[ 3] = (m[ 3]*v[ 0]) + (m[ 7]*v[ 1]) + (m[11]*v[ 2]) + (m[15]*v[ 3]);
		_m[ 4] = (m[ 0]*v[ 4]) + (m[ 4]*v[ 5]) + (m[ 8]*v[ 6]) + (m[12]*v[ 7]);
		_m[ 5] = (m[ 1]*v[ 4]) + (m[ 5]*v[ 5]) + (m[ 9]*v[ 6]) + (m[13]*v[ 7]);
		_m[ 6] = (m[ 2]*v[ 4]) + (m[ 6]*v[ 5]) + (m[10]*v[ 6]) + (m[14]*v[ 7]);
		_m[ 7] = (m[ 3]*v[ 4]) + (m[ 7]*v[ 5]) + (m[11]*v[ 6]) + (m[15]*v[ 7]);
		_m[ 8] = (m[ 0]*v[ 8]) + (m[ 4]*v[ 9]) + (m[ 8]*v[10]) + (m[12]*v[11]);
		_m[ 9] = (m[ 1]*v[ 8]) + (m[ 5]*v[ 9]) + (m[ 9]*v[10]) + (m[13]*v[11]);
		_m[10] = (m[ 2]*v[ 8]) + (m[ 6]*v[ 9]) + (m[10]*v[10]) + (m[14]*v[11]);
		_m[11] = (m[ 3]*v[ 8]) + (m[ 7]*v[ 9]) + (m[11]*v[10]) + (m[15]*v[11]);
		_m[12] = (m[ 0]*v[12]) + (m[ 4]*v[13]) + (m[ 8]*v[14]) + (m[12]*v[15]);
		_m[13] = (m[ 1]*v[12]) + (m[ 5]*v[13]) + (m[ 9]*v[14]) + (m[13]*v[15]);
		_m[14] = (m[ 2]*v[12]) + (m[ 6]*v[13]) + (m[10]*v[14]) + (m[14]*v[15]);
		_m[15] = (m[ 3]*v[12]) + (m[ 7]*v[13]) + (m[11]*v[14]) + (m[15]*v[15]);
		return _m;
	}	//	inline CMatrix operator *(const CMatrix &v) const
	inline CMatrix operator /(const CMatrix &v) const { float _m[16]; for (int i=0; i<16; i++) _m[i] = m[i] / v.m[i]; return CMatrix(_m); }
	inline CMatrix operator +(const float f) const { float _m[16]; for (int i=0; i<16; i++) _m[i] = m[i] + f; return CMatrix(_m); }
	inline CMatrix operator -(const float f) const { float _m[16]; for (int i=0; i<16; i++) _m[i] = m[i] - f; return CMatrix(_m); }
	inline CMatrix operator *(const float f) const { float _m[16]; for (int i=0; i<16; i++) _m[i] = m[i] * f; return CMatrix(_m); }
	inline CMatrix operator /(const float f) const { float _m[16]; for (int i=0; i<16; i++) _m[i] = m[i] / f; return CMatrix(_m); }

	float m[16];

protected:
private:
};	//	class CMatrix;

CVector3 Random(const CVector3 &Max, const CVector3 &Min);
CMatrix Identity(void);
CMatrix Translate(const CVector3 &v);
CMatrix Scale(const CVector3 &v);
CMatrix RotateX(const float angle);
CMatrix RotateY(const float angle);
CMatrix RotateZ(const float angle);
CMatrix Perspective(const float fovy, const float aspect, float znear, float zfar);
CMatrix Ortho(const float left, const float right, const float bottom, const float top,
	const float znear, const float zfar);
float Determinant(const CMatrix &m);
CMatrix Inverse(const CMatrix &m);
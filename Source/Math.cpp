#include "PreCompiled.h"

CVector3 Random(const CVector3 &Max, const CVector3 &Min) {
	CVector3 v;
	v.x = ((float)rand()/(float)RAND_MAX)*(Max.x-Min.x)+Min.x;
	v.y = ((float)rand()/(float)RAND_MAX)*(Max.y-Min.y)+Min.y;
	v.z = ((float)rand()/(float)RAND_MAX)*(Max.z-Min.z)+Min.z;
	return v;
}	//	CVector3 Random(const CVector3 &Max, const CVector3 &Min)

CMatrix Identity(void) {
	CMatrix m;
	return m;
}	//	CMatrix Identity(void)

CMatrix Translate(const CVector3 &v) { 
	CMatrix m; m[12] = v.x;
	m[13] = v.y; m[14] = v.z;
	return m;
}	//	CMatrix Translate(const CVector3 &v)

CMatrix Scale(const CVector3 &v) { 
	CMatrix m;
	m[0] = v.x;
	m[5] = v.y;
	m[10] = v.z;
	return m;
}	//	CMatrix Scale(const CVector3 &v)

CMatrix RotateX(const float angle) {
	float c = cosf(DegToRad(angle));
	float s = sinf(DegToRad(angle));
	CMatrix m;
	m[5] = m[10] = c; m[6] = s; m[9] = -s;
	return m;
}	//	CMatrix RotateX(const float angle)

CMatrix RotateY(const float angle) {
	float c = cosf(DegToRad(angle));
	float s = sinf(DegToRad(angle));
	CMatrix m;
	m[0] = m[10] = c; m[8] = s; m[2] = -s;
	return m;
}	//	CMatrix RotateY(const float angle)

CMatrix RotateZ(const float angle) {
	float c = cosf(DegToRad(angle));
	float s = sinf(DegToRad(angle));
	CMatrix m;
	m[0] = m[5] = c; m[1] = s; m[4] = -s;
	return m;
}	//	CMatrix RotateZ(const float angle)

CMatrix Perspective(const float fovy, const float aspect, float znear, float zfar) {
	float left, right, top, bottom;
    top    = znear * tanf(fovy*PI/360.0f);
    bottom = -top;
    left   = bottom * aspect;
    right  = top * aspect;

	CMatrix m;
    m[ 0] = 2.0f*znear / (right-left);
    m[ 5] = 2.0f*znear / (top-bottom);
    m[ 8] = (right+left) / (right-left);
    m[ 9] = (top+bottom) / (top-bottom);
    m[10] = (zfar+znear) / (znear-zfar);
    m[11] = -1.0f;
    m[14] = 2.0f*zfar*znear / (znear-zfar);
	m[15] = 0.0f;
	return m;
}	//	CMatrix Perspective(...)

CMatrix Ortho(const float left, const float right, const float bottom, const float top,
	const float znear, const float zfar) {  
	CMatrix m;
    m[ 0]  = 2.0f / (right-left);
    m[ 5]  = 2.0f / (top-bottom);
    m[10] = -2.0f / (zfar-znear);  
    m[12] = -(right+left) / (right-left);
    m[13] = -(top+bottom) / (top-bottom);
    m[14] = -(zfar+znear) / (zfar-znear);
    m[15] = 1.0f;
	return m;
}	//	CMatrix Ortho(...)

float Determinant(const CMatrix &m) {
	float	r  = m[0] * m[5] * m[10];
			r += m[4] * m[9] * m[2];
			r += m[8] * m[1] * m[6];
			r -= m[8] * m[5] * m[2];
			r -= m[4] * m[1] * m[10];
			r -= m[0] * m[9] * m[6];
	return r;
}	//	float Determinant(const CMatrix &m)

CMatrix Inverse(const CMatrix &m) {
	CMatrix r;
	float det = Determinant(m);
	if (!det) return r;

	det = 1.0f / det;

	r[0] =  (m[ 5]*m[10]-m[ 9]*m[ 6])*det;
	r[1] = -(m[ 1]*m[10]-m[ 9]*m[ 2])*det;
	r[2] =  (m[ 1]*m[ 6]-m[ 5]*m[ 2])*det;
	r[3] = 0.0f;
	r[4] = -(m[ 4]*m[10]-m[ 8]*m[ 6])*det;
	r[5] =  (m[ 0]*m[10]-m[ 8]*m[ 2])*det;
	r[6] = -(m[ 0]*m[ 6]-m[ 4]*m[ 2])*det;
	r[7] = 0.0f;
	r[8] =  (m[ 4]*m[ 9]-m[ 8]*m[ 5])*det;
	r[9] = -(m[ 0]*m[ 9]-m[ 8]*m[ 1])*det;
	r[10] = (m[ 0]*m[ 5]-m[ 4]*m[ 1])*det;
	r[11] = 0.0f;
	r[12] = -(m[12]*r[ 0]+m[13]*r[ 4]+m[14]*r[ 8]);
	r[13] = -(m[12]*r[ 1]+m[13]*r[ 5]+m[14]*r[ 9]);
	r[14] = -(m[12]*r[ 2]+m[13]*r[ 6]+m[14]*r[10]);
	r[15] = 1.0f;

	return r;
}	//	CMatrix Inverse(const CMatrix &m)
//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/Matrix.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_MATRIX_H__
#define __GELIB_MATH_MATRIX_H__

#include <GELib/GeConfig.h>
#include <GELib/Math/Vector.h>

namespace GELib
{


class CPlane;
class CQuaternion;

class CMatrix
{
public:
	union
	{
		struct
		{
			float _00, _01, _02, _03;
			float _10, _11, _12, _13;
			float _20, _21, _22, _23;
			float _30, _31, _32, _33;
		};
		struct
		{
			float m_vRow0[4];
			float m_vRow1[4];
			float m_vRow2[4];
			float m_vRow3[4];
		};
		float m_m[4][4];
		float m_m16[16];
	};


    CMatrix() { SetIdentity(); }
    CMatrix(const CMatrix &m) : _00(m._00), _01(m._01), _02(m._02), _03(m._03), _10(m._10), _11(m._11), _12(m._12), _13(m._13), _20(m._20), _21(m._21), _22(m._22), _23(m._23), _30(m._30), _31(m._31), _32(m._32), _33(m._33) {}
    CMatrix(const float m16[16]) : _00(m16[0]), _01(m16[1]), _02(m16[2]), _03(m16[3]), _10(m16[4]), _11(m16[5]), _12(m16[6]), _13(m16[7]), _20(m16[8]), _21(m16[9]), _22(m16[10]), _23(m16[11]), _30(m16[12]), _31(m16[13]), _32(m16[14]), _33(m16[15]) {}
    CMatrix(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) : _00(m00), _01(m01), _02(m02), _03(m03), _10(m10), _11(m11), _12(m12), _13(m13), _20(m20), _21(m21), _22(m22), _23(m23), _30(m30), _31(m31), _32(m32), _33(m33) {}
    CMatrix(const CQuaternion &q, const CVector3 &row3) { Set(q, row3); }
    ~CMatrix() {}

    void Set(float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33);
    void Set(const CMatrix &m16) { MatrixAssign(m_m16, m16.m_m16); }
    void Set(const CQuaternion &q, const CVector3 &row3);

    void SetRow(int iRowIdx, const CVector3 &vect);
    void GetRow(int iRowIdx, CVector3 &vect);
    void SetCol(int iColIdx, const CVector3 &vect);
    void GetCol(int iColIdx, CVector3 &vect);

    void SetRow(int iRowIdx, const CVector4 &vect);
    void GetRow(int iRowIdx, CVector4 &vect);
    void SetCol(int iColIdx, const CVector4 &vect);
    void GetCol(int iColIdx, CVector4 &vect);

    void SetRotationX(float radian);
    void SetRotationY(float radian);
    void SetRotationZ(float radian);
    void SetRotation(const CQuaternion &q);
    void SetScale(const CVector3 &scale);

    void SetZero();
    void SetIdentity();

    CMatrix Transpose() const;
    void TransposeSelf();
    CMatrix Inverse() const;
    void InverseSelf();
    CMatrix InverseFast() const;
    void InverseFastSelf() ;

    void Scale(float fX, float fY, float fZ);
	void Scale(const CVector3 &v)  { Scale(v.x, v.y, v.z); }

    void TranslateX(float fDistance);
    void TranslateY(float fDistance);
    void TranslateZ(float fDistance);

    void Translate(float fX, float fY, float fZ, bool bPostMul);
    void Translate(const CVector3 &v, bool bPostMul)   { Translate(v.x, v.y, v.z, bPostMul); }

    void RotateX(float radian, bool bPostMul);
    void RotateY(float radian, bool bPostMul);
    void RotateZ(float radian, bool bPostMul);
    void Rotate(const CVector3 &axis, float radian, bool bPostMul);

    void Perspective(float fovy_in_radian, float aspect, float znear, float zfar);
    void PerspectiveX(float fovx_in_radian, float aspect, float znear, float zfar);
    void Perspective(float left,  float right, float bottom, float top, float znear, float zfar);

    void Ortho(float w,  float h, float znear, float zfar);
    void Ortho(float left,  float right, float bottom, float top, float znear, float zfar);

    CMatrix operator+(const CMatrix &m) const;
    CMatrix operator-(const CMatrix &m) const;
    CMatrix operator*(const CMatrix &m) const;
    CMatrix operator*(float f) const;
    CMatrix operator/(float f) const;
    CMatrix & operator+=(const CMatrix &m);
    CMatrix & operator-=(const CMatrix &m);
    CMatrix & operator*=(const CMatrix &m);
    CMatrix operator*=(float f);
    CMatrix operator/=(float f);

    bool operator==(const CMatrix &m) const;
    bool operator!=(const CMatrix &m) const;
};

inline void CMatrix::SetZero()
{
	MatrixSetZero(m_m16);
}

inline void CMatrix::SetIdentity()
{
	MatrixSetUnit(m_m16);
}

inline CMatrix CMatrix::Transpose() const
{
	return CMatrix(_00, _10, _20, _30, _01, _11, _21, _31, _02, _12, _22, _32, _03, _13, _23, _33);
}

inline void CMatrix::TransposeSelf()
{
	float tmp = _01; _01 = _10; _10 = tmp;
	tmp = _02; _02 = _20; _20 = tmp;
	tmp = _03; _03 = _30; _30 = tmp;
	tmp = _12; _12 = _21; _21 = tmp;
	tmp = _13; _13 = _31; _31 = tmp;
	tmp = _23; _23 = _32; _32 = tmp;
}

inline void CMatrix::SetScale(const CVector3& scale)
{
	MatrixSetScale(m_m16, scale.x, scale.y, scale.z);
}

inline void CMatrix::Scale(float fX, float fY, float fZ)
{
	MatrixScale(m_m16, fX, fY, fZ);
}

inline void CMatrix::SetRotationX(float radian)
{
	MatrixSetRotationX(m_m16, radian);
}

inline void CMatrix::SetRotationY(float radian)
{
	MatrixSetRotationY(m_m16, radian);
}

inline void CMatrix::SetRotationZ(float radian)
{
	MatrixSetRotationZ(m_m16, radian);
}

inline void CMatrix::RotateX(float radian, bool bPostMul)
{
	MatrixRotateX(m_m16, radian, bPostMul);
}

inline void CMatrix::RotateY(float radian, bool bPostMul)
{
	MatrixRotateY(m_m16, radian, bPostMul);
}

inline void CMatrix::RotateZ(float radian, bool bPostMul)
{
	MatrixRotateZ(m_m16, radian, bPostMul);
}

inline void CMatrix::Rotate(const CVector3 &axis, float radian, bool bPostMul)
{
	MatrixRotate(m_m16, m_m16, axis.m_v, radian, bPostMul);
}

inline CMatrix CMatrix::Inverse() const
{
	CMatrix m16;
	MatrixMakeInverse(m16.m_m16, m_m16);
	return m16;
}

inline void CMatrix::InverseSelf()
{
	MatrixMakeInverse(m_m16, m_m16);
}

inline CMatrix CMatrix::InverseFast() const
{
	CMatrix dest;
	MatrixMakeInverseFast(dest.m_m16, m_m16);
	return dest;
}

inline void CMatrix::InverseFastSelf()
{
	float m16[16];
	MatrixMakeInverseFast(m16, m_m16);
	*this = m16;
}

inline CMatrix CMatrix::operator+(const CMatrix &m) const
{
   CMatrix M;
   MatrixAdd(M.m_m16, m_m16, m.m_m16);
   return M;
}

inline CMatrix CMatrix::operator-(const CMatrix &m) const
{
   CMatrix M;
   MatrixSub(M.m_m16, m_m16, m.m_m16);
   return M;
}

inline CMatrix CMatrix::operator*(const CMatrix &m) const
{
   CMatrix dest;
   MatrixMul(dest.m_m16, m_m16, m.m_m16);
   return dest;
}

inline CMatrix CMatrix::operator*(float f) const
{
   CMatrix M;
   MatrixMul(M.m_m16, m_m16, f);
   return M;
}

inline CMatrix CMatrix::operator/(float f) const
{
   CMatrix M;
   MatrixDiv(M.m_m16, m_m16, f);
   return M;
}

inline CMatrix & CMatrix::operator+=(const CMatrix &m)
{
   MatrixAdd(m_m16, m_m16, m.m_m16);
   return *this;
}

inline CMatrix & CMatrix::operator-=(const CMatrix &m)
{
   MatrixSub(m_m16, m_m16, m.m_m16);
   return *this;
}

inline CMatrix & CMatrix::operator*=(const CMatrix &m)
{
   MatrixMul(m_m16, m_m16, m.m_m16);
   return *this;
}

inline CMatrix CMatrix::operator*=(float f)
{
   MatrixMul(m_m16, m_m16, f);
   return *this;
}

inline CMatrix CMatrix::operator/=(float f)
{
   MatrixDiv(m_m16, m_m16, f);
   return *this;
}

inline bool CMatrix::operator==(const CMatrix &m) const
{
   return MatrixIsEqual(m_m16, m.m_m16);
}

inline bool CMatrix::operator!=(const CMatrix &m) const
{
   return !MatrixIsEqual(m_m16, m.m_m16);
}

inline const CVector3& MatX(const CMatrix &m)
{
	return *(const CVector3*)&m._00;
}

inline const CVector3& MatY(const CMatrix &m)
{
	return *(const CVector3*)&m._10;
}

inline const CVector3& MatZ(const CMatrix &m)
{
	return *(const CVector3*)&m._20;
}

inline const CVector3& MatT(const CMatrix &m)
{
	return *(const CVector3*)&m._30;
}

inline const CVector3& MatAxis(const CMatrix &m, int axis)
{
	return *(const CVector3*)m.m_m[axis];
}

inline CVector3& MatX(CMatrix &m)
{
	return *(CVector3*)&m._00;
}

inline CVector3& MatY(CMatrix &m)
{
	return *(CVector3*)&m._10;
}

inline CVector3& MatZ(CMatrix &m)
{
	return *(CVector3*)&m._20;
}

inline CVector3& MatT(CMatrix &m)
{
	return *(CVector3*)&m._30;
}

inline CVector3& MatAxis(CMatrix &m, int axis)
{
	return *(CVector3*)m.m_m[axis];
}

// result must not be a or b
void MatrixMultiply4x3(CMatrix &result, const CMatrix &a, const CMatrix &b);
void MatrixMultiply3x3(CMatrix &result, const CMatrix &a, const CMatrix &b);

// rot.x: yaw (rad)
// rot.y: pitch (rad)
// rot.z: roll (rad)
// out = Ry(roll) * Rx(pitch) * Rz(yaw)
// convert between yaw pitch roll and character matrix (front is -Y)
GELIB_API void RotationToMatrixYXZ(const CVector3 &rot, CMatrix &m);
GELIB_API void MatrixToRotationYXZ(const CMatrix &m, CVector3 &rot);

// rot.x: yaw (rad)
// rot.y: pitch (rad)
// rot.z: roll (rad)
// out = Rz(roll) * Rx(pitch) * Ry(yaw)
// convert between yaw pitch roll and camera matrix (front is -Z)
GELIB_API void RotationToMatrixZXY(const CVector3 &rot, CMatrix &m);
GELIB_API void MatrixToRotationZXY(const CMatrix &m, CVector3 &rot);

// rot.x: yaw (rad)
// rot.y: pitch (rad)
// rot.z: roll (rad)
// out = Rz(roll) * Ry(pitch) * Rx(yaw)
// convert between yaw pitch roll and camera matrix (front is -Z)
GELIB_API void RotationToMatrixZYX(const CVector3 &rot, CMatrix &m);
GELIB_API void MatrixToRotationZYX(const CMatrix &m, CVector3 &rot);

GELIB_API void MakeReflectMatrix(CMatrix &m, const CPlane &plane);

GELIB_API void OrthogonolizeMatrix(CMatrix &m, bool normalize);

GELIB_API void ProjectionMatrixToFov(const CMatrix &projMatrix, float &fov, float &znear, float &zfar);	// in radian

GELIB_API void ClipProjectionMatrix(CMatrix &outMatrix, const CMatrix &viewMatrix, const CMatrix &projMatrix, const CPlane &clipPlane);


}//GELib

#endif//__GELIB_MATH_MATRIX_H__

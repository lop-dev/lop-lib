//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Math/MathImpl.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_MATH_MATHIMPL_H__
#define __GELIB_MATH_MATHIMPL_H__

#include <string.h>
#include <stdlib.h>


namespace GELib
{

#define GMH_EPSILON     (0.00001f)

float FastInvSqrt(float x);

inline void Vector3Assign(float dest[3], const float v1[3]) { memcpy(dest, v1, sizeof(float) * 3); }
void Vector3MulM16(float dest[3], const float v3[3], const float m16[16]);

inline void Vector4Assign(float dest[4], const float v1[4]) { memcpy(dest, v1, sizeof(float) * 4); }
void Vector4MulM16(float dest[4], const float v4[4], const float m16[16]);

inline void QuaternionAssign(float qDest[4], const float q1[4]) { memcpy(qDest, q1, sizeof(float) * 4); }
void QuaternionToMatrix(float m[16], const float vQt[4]);
void QuaternionFromMatrix(float vQt[4], const float m[16]);
void QuaternionMul(float dest[4], const float q1[4], const float q2[4]);

void MatrixSetUnit(float dest[16]);
inline void MatrixSetZero(float dest[16]) { ::memset(dest, 0, 16 * sizeof(float)); }
inline void MatrixAssign(float dest[16], const float m1[16]) { ::memcpy(dest, m1, sizeof(float)*16); }

void MatrixMul(float dest[16], const float m1[16], const float m2[16]);
void MatrixMul(float dest[16], const float m1[16], float f);
void MatrixDiv(float dest[16], const float m1[16], float f);
void MatrixAdd(float dest[16], const float m1[16], const float m2[16]);
void MatrixSub(float dest[16], const float m1[16], const float m2[16]);

void MatrixRotateX(float dest[16], float radian, bool bPostMul);
void MatrixRotateY(float dest[16], float radian, bool bPostMul);
void MatrixRotateZ(float dest[16], float radian, bool bPostMul);
void MatrixRotate(float dest[16], const float m1[16], const float axis[3], float radian, bool bPostMul);
void MatrixSetRotationX(float dest[16], float radian);
void MatrixSetRotationY(float dest[16], float radian);
void MatrixSetRotationZ(float dest[16], float radian);

void MatrixMakeInverse(float dest[16], const float m1[16]);
void MatrixMakeInverseFast(float dest[16], const float m1[16]);

bool MatrixIsEqual(const float m1[16], const float m2[16]);
void MatrixScale(float dest[16], float fScaleX, float fScaleY, float fScaleZ);
void MatrixSetScale(float dest[16], float fScaleX, float fScaleY, float fScaleZ);

bool GMh2DIsVertexIntersectRectangle(const float v[2], const float rect[4]);
bool GMh2DIsLineIntersectLine(const float v0[2], const float v1[2], const float u0[2], const float u1[2]);
bool GMh2DIsLineIntersectLine(const float v0[2], const float v1[2], const float v2[2], const float v3[2], float *pTheT);
bool GMh2DGetCrossPointLineOnLine(const float v0[2], const float v1[2], const float u0[2], const float u1[2], float crosspoint[2]); // get cross
bool GMh2DIsLineIntersectRectangle(const float v0[2], const float v1[2], const float rect[4]);
bool GMh2DIsLineIntersectCircle(const float v0[2], const float v1[2], const float c1[2], float fRadius);
float GMh2DPointToLineDistanceSqr(const float v0[2], const float v1[2], const float c1[2]);
int GMh2DGetLine_CircleIntersection(const float v0[2], const float v1[2], const float c1[2], float fRadius, float intersectionp0[2],
                                             float intersectionp1[2]);

bool GMhIsLineIntersectSquareXY(const float v0[3], const float v1[3], const float square[6]);
bool GMhIsLineIntersectSquareXZ(const float v0[3], const float v1[3], const float square[6]);
bool GMhIsLineIntersectSquareYZ(const float v0[3], const float v1[3], const float square[6]);
bool GMhIsLineIntersectPlaneXY(const float v0[3], const float v1[3], float fZ,
                                           float fIntersectPoint[3]=0, float *pTheT=0);

bool GMhIsLineIntersectTriangle(const float v0[3], const float v1[3], const float triangle[9],
                                            bool bFilterBackFace=false, float fIntersectPoint[3]=0, float *pTheT=0);
bool GMhIsLineIntersectWithTriangle(const float v0[3], const float v1[3], const float triV0[3], const float triV1[3], const float triV2[3],
                                               bool bFilterBackFace=false,  float fIntersectPoint[3]=0, float *pTheT=0);
bool GMhIsLineIntersectRectangle(const float v0[3], const float v1[3], const float rect[12],
                                           float fIntersectPoint[3]=0, float *pTheT=0);
bool GMhIsRayHitPlane(const float p[3], const float dir[3], const float planeN[3], const float planeD,
                                  float fIntersectPoint[3], float *pTheT);
bool GMhIsRayHitTriangle(const float p[3], const float dir[3], const float triangle[9], bool bFilterBackFace,
                                     float fIntersectPoint[3]=0, float *pTheT=0);
bool GMhIsRayHitAABB(const float p[3], const float dir[3], const float AABB[6],
                                 float fCollidedPoint[3]=0, float* fTheT=0); // AABB[6] --> x0, y0, z0, x1, y1, z1,
bool GMhIsRayHitWithAABB(const float p[3], const float dir[3], const float fMin[3], const float fMax[3],
                                     float fCollidedPoint[3]=0, float* fTheT=0);
bool GMhIsRayIntersectHollowAABB(const float vPos[3], const float vDir[3], const float AABB[6],
                                             float faCollidedPoint[3]=0, float *t=0); // AABB[6] --> x0, y0, z0, x1, y1, z1,
bool GMhIsRayIntersectOBB(const float fPos[3], const float fDir[3],
                                      const float fOBBPos[3], const float fOBBOrient[9], const float fOBBExt[3],
                                      float fCollidedPoint[3]=0, float* fTheT=0);
bool GMhIsRayIntersectOBBM12(const float fPos[3], const float fDir[3], const float fOBBMatrix12[12], const float fOBBExt[3],
                                         float fCollidedPoint[3]=0, float* fTheT=0);
bool GMhIsRayHitSphere(const float fRayPos[3], const float fRayDir[3], const float fSphereCenter[3], float fSphereRadius,
                                  float fCollidedPoint[3]=0, float* fTheT=0);

inline bool GMhIsVertexIntersectAABB(const float v[3], const float AABB[6]) // AABB[6] --> x0, y0, z0, x1, y1, z1,
{
    if (v[0] < AABB[0] || v[0] > AABB[3]) return false;
    if (v[1] < AABB[1] || v[1] > AABB[4]) return false;
    if (v[2] < AABB[2] || v[2] > AABB[5]) return false;
    return true;
}

inline bool GMhIsVertexIntersectAABB(const float v[3], const float fMin[3], const float fMax[3])
{
    if (v[0] < fMin[0] || v[0] > fMax[0]) return false;
    if (v[1] < fMin[1] || v[1] > fMax[1]) return false;
    if (v[2] < fMin[2] || v[2] > fMax[2]) return false;
    return true;
}

bool GMhIsVertexIntersectSphere(const float fSpherePos[3], float fR, const float fVertex[3]);
bool GMhIsLineIntersectAABB(const float v0[3], const float v1[3], const float AABB[6]); // AABB[6] --> x0, y0, z0, x1, y1, z1,
bool GMhIsLineIntersectAABB(const float v0[3], const float v1[3], const float AABB[6],
                                       float faCollidedPoint[3], float *rt=0); // AABB[6] --> x0, y0, z0, x1, y1, z1,

inline bool GMhIsLineIntersectWithAABB(const float v0[3], const float v1[3], const float fMin[3], const float fMax[3],
                                                  float faCollidedPoint[3]=0, float *rt=0)
{
    float AABB[6] = { fMin[0], fMin[1], fMin[2], fMax[0], fMax[1], fMax[2] };
    return GMhIsLineIntersectAABB(v0, v1, AABB, faCollidedPoint, rt);
}

bool GMhIsLineIntersectHollowAABB(const float v0[3], const float v1[3], const float AABB[6],
                                             float faCollidedPoint[3]=0, float *t=0); // AABB[6] --> x0, y0, z0, x1, y1, z1,
bool GMhIsLineIntersectOBB(const float v0[3], const float v1[3],
                                      const float fOBBPos[3], const float fOBBOrient[9], const float fOBBExt[3],
                                      float fCollidedPoint[3]=0, float* fTheT=0);
bool GMhIsAABBIntersectTriangle(const float AABB[6], const float triangle[9]);
bool GMhIsAABBIntersectSphere(const float AABB[6], const float pos[3], float r_square);

bool GMhIsAABBIntersectWithAABB(const float AABB1Min[3], const float AABB1Max[3],
                                                  const float AABB2Min[3], const float AABB2Max[3]);

bool GMhIsAABBIntersectAABB(const float fExtA[3], const float fPosA[3],
                                              const float fExtB[3], const float fPosB[3]);

bool GMhIsAABBContainAABB(const float AABB1Min[3], const float AABB1Max[3],
                                            const float AABB2Min[3], const float AABB2Max[3]);

bool GMhIsAABBIntersectWithPlane(const float AABB1Min[3], const float AABB1Max[3],
                                            const float faNormal[3], float fDistance, int* piSide=0);

bool GMhIsOBBIntersectOBB(const float fExtA[3], const float fPosA[3], const float fBasisA[9],
                                     const float fExtB[3], const float fPosB[3], const float fBasisB[9]);

bool GMhIsOBBIntersectOBB(const float fExtA[3], const float fMx12A[12],
                                     const float fExtB[3], const float fMx12B[12]);

bool GMhIsOBBIntersectPlane(const float fExtA[3], const float fPosA[3], const float fBasisA[9],
									    const float faNormal[3], float fDistance, int *piSide=0);

bool GMhIsOBBIntersectTriangle(const float fExtA[3], const float fPosA[3], const float fBasisA[9], const float triangle[9],const float *fOBBInvM12 = 0);

//bool GMhIsOBBContainOBB(const float fExtA[3], const float fPosA[3], const float fBasisA[9],
//								   const float fExtB[3], const float fPosB[3], const float fBasisB[9], const float *fBInvM12 = 0);


}//GELib

#endif//__GELIB_MATH_MATHIMPL_H__

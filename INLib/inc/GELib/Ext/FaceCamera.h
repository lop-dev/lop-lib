//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/FaceCamera.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_FACECAMERA_H__
#define __GELIB_EXT_FACECAMERA_H__

#include <GELib/Math/GeMath.h>

namespace GELib
{

enum EFaceCameraMode
{
    FACE_CAMERA_NONE = 0,
    FACE_CAMERA_XYZ,
    FACE_CAMERA_LOCAL,
    FACE_CAMERA_Z_FIXED,
};

CMatrix ComputeFaceCameraMatrix(EFaceCameraMode mode, const CMatrix &matrix, const CMatrix &cameraMatrix);

}//GELib

#endif//__GELIB_EXT_FACECAMERA_H__

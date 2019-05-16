//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Fx/FxTypes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_FX_FXTYPES_H__
#define __GELIB_FX_FXTYPES_H__


namespace GELib
{


// Fx compoment class id
const int FXCLASS_BASE = 1;
const int FXCLASS_BOX = 2;
const int FXCLASS_SPHERE = 3;
const int FXCLASS_CYLINDER = 4;
const int FXCLASS_MESH = 5;
const int FXCLASS_CHARACTER = 6;
const int FXCLASS_PARTICLE = 7;
const int FXCLASS_LIGHT = 8;
const int FXCLASS_SOUND = 9;
const int FXCLASS_EVENT = 10;
const int FXCLASS_TRAIL = 11;
const int FXCLASS_DECAL = 12;
const int FXCLASS_BOARD = 13;
const int FXCLASS_CAMERA = 14;
const int FXCLASS_ANIMATION = 15;
const int FXCLASS_FX = 16;
const int FXCLASS_LINE = 17;
const int FXCLASS_SOUND2D = 18;
const int FXCLASS_DIRECTOR = 19;
const int FXCLASS_IMAGE = 20;
const int FXCLASS_TWEEN = 21;
const int FXCLASS_RAYCAST = 22;
const int FXCLASS_ACT = 23;
const int FXCLASS_AREA = 24;
const int FXCLASS_COLLISION = 25;

const int FXATTACH_NONE = 0;
const int FXATTACH_POSITION = 1 << 0;
const int FXATTACH_ROTATION = 1 << 1;
const int FXATTACH_SCALE = 1 << 2;
const int FXATTACH_POSITION_LOCAL = 1 << 3;

enum EFxAttachTime
{
    FXATTACH_ALWAYS = 0,
    FXATTACH_ONCE = 1
};

enum EFxRotateOrder
{
    FXROTATE_XYZ = 0,
    FXROTATE_XZY = 1,
    FXROTATE_YXZ = 2,
    FXROTATE_YZX = 3,
    FXROTATE_ZXY = 4,
    FXROTATE_ZYX = 5,
    FXROTATE_QUATERNION = 6,
};

enum EFxComponentOffsetMode
{
    FXOFFSET_NONE = 0,
    FXOFFSET_LOCAL = 1,
    FXOFFSET_GLOBAL = 2,
};

enum EFxHaltBehavior
{
	FXHALT_FADE_OUT = 0,
	FXHALT_HIDE = 1,
	FXHALT_PLAY = 2
};



}//GELib

#endif//__GELIB_FX_FXTYPES_H__

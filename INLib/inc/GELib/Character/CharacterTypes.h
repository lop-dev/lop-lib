//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/CharacterTypes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_CHARACTERTYPES_H__
#define __GELIB_CHARACTER_CHARACTERTYPES_H__

namespace GELib
{


static const short ANIM_PARAM_SPEED = 101;
static const short ANIM_PARAM_PLAY_RANGE = 102;
static const short ANIM_PARAM_LOOP_RANGE = 103;
static const short ANIM_PARAM_LOOP = 104;
static const short ANIM_PARAM_TRANSITION = 105;
static const short ANIM_PARAM_HURTCOLLISION = 106;

static const int TRANSITION_MODE_NONE = 0;
static const int TRANSITION_MODE_BEGIN_WAIT = 1;
static const int TRANSITION_MODE_BEGIN_PLAY = 2;
static const int TRANSITION_MODE_SYNC_PLAY = 3;

static const int CHAR_DRAWFLAG_BONE = 1 << 0;
static const int CHAR_DRAWFLAG_SHAPE = 1 << 1;
static const int CHAR_DRAWFLAG_DYNAMIC_BONE = 1 << 2;
static const int CHAR_DRAWFLAG_IK_BONE = 1 << 3;
static const int CHAR_DRAWFLAG_SKIN_BOX = 1 << 4;
static const int CHAR_DRAWFLAG_COLLISION_SIZE = 1 << 5;

enum ECharShapeType
{
	CHAR_SHAPE_NULL = 0,
    CHAR_SHAPE_SPHERE,
    CHAR_SHAPE_BOX,
    CHAR_SHAPE_CAPSULE
};



}//GELib

#endif//__GELIB_CHARACTER_CHARACTERTYPES_H__

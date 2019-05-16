//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/GroupTypes.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_GROUPTYPES_H__
#define __GELIB_EXT_GROUPTYPES_H__

namespace GELib
{


const int LIGHTGROUP_MESH = 1 << 0;
const int LIGHTGROUP_CHARACTER = 1 << 1;
const int LIGHTGROUP_FX = 1 << 2;
const int LIGHTGROUP_TREE = 1 << 3;
const int LIGHTGROUP_INSTANCE_MESH = 1 << 4;
const int LIGHTGROUP_TERRAIN = 1 << 5;
const int LIGHTGROUP_GRASS = 1 << 6;
const int LIGHTGROUP_WATER = 1 << 7;
const int LIGHTGROUP_GROUND = 1 << 8;
const int LIGHTGROUP_CUSTOM_1 = 1 << 12;
const int LIGHTGROUP_CUSTOM_2 = 1 << 13;
const int LIGHTGROUP_CUSTOM_3 = 1 << 14;
const int LIGHTGROUP_CUSTOM_4 = 1 << 15;

const int LODGROUP_DEFAULT = 0;
const int LODGROUP_TERRAIN = 1;
const int LODGROUP_GRASS = 2;
const int LODGROUP_INSTANCE_MESH = 3;

const int COLLISIONGROUP_NONE = 0;
const int COLLISIONGROUP_SCENE = 1;
const int COLLISIONGROUP_CHARACTER = 2;
const int COLLISIONGROUP_ITEM = 3;
const int COLLISIONGROUP_FX = 4;

const int COLLISIONMASK_BLOCK_PC = 1 << 0;
const int COLLISIONMASK_BLOCK_NPC = 1 << 1;
const int COLLISIONMASK_BLOCK_CHARACTER = COLLISIONMASK_BLOCK_PC | COLLISIONMASK_BLOCK_NPC;
const int COLLISIONMASK_BLOCK_CAMERA = 1 << 2;
const int COLLISIONMASK_BLOCK_SKILL = 1 << 3;
const int COLLISIONMASK_BLOCK_PROJECTILE = 1 << 4;
const int COLLISIONMASK_WALKABLE = 1 << 5;
const int COLLISIONMASK_BLOCK_FX = 1 << 6;
const int COLLISIONMASK_BLOCK_IK = 1 << 7;

const int COLLISIONMASK_SCENE = 1 << 10;
const int COLLISIONMASK_CHARACTER = 1 << 11;
//const int COLLISIONMASK_TRIGGER = 1 << 12;
const int COLLISIONMASK_HIT = 1 << 13;

const int PHYSICSOBJECT_SCENE = 1;
const int PHYSICSOBJECT_ENTITY = 2;


}//GELib


#endif//__GELIB_EXT_GROUPTYPES_H__

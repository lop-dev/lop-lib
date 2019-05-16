//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/DefaultPhysicsMaterial.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_DEFAULTPHYSICSMATERIAL_H__
#define __GELIB_EXT_DEFAULTPHYSICSMATERIAL_H__

namespace GELib
{

const int PHYSICSMATERIAL_DEFAULT  = 0;
const int PHYSICSMATERIAL_GRASS  = 1;
const int PHYSICSMATERIAL_BUSH  = 2;
const int PHYSICSMATERIAL_SAND   = 3;
const int PHYSICSMATERIAL_SOIL    = 4;
const int PHYSICSMATERIAL_MUD    = 5;
const int PHYSICSMATERIAL_WATER  = 6;
const int PHYSICSMATERIAL_SNOW   = 7;
const int PHYSICSMATERIAL_ICE  = 8;
const int PHYSICSMATERIAL_METAL  = 9;
const int PHYSICSMATERIAL_ROCK   = 10;
const int PHYSICSMATERIAL_WOOD   = 11;
const int PHYSICSMATERIAL_CLOTH  = 12;
const int PHYSICSMATERIAL_PLASTIC  = 13;
const int PHYSICSMATERIAL_MAX    = 14;

class CPhysicsWorld;
void CreateDefaultPhysicsMaterials(CPhysicsWorld *physicsWorld);

}//GELib


#endif//__GELIB_EXT_DEFAULTPHYSICSMATERIAL_H__

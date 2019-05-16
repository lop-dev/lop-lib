//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Ext/Act.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_EXT_ACT_H__
#define __GELIB_EXT_ACT_H__

#include <GELib/Math/GeMath.h>
#include <map>
#include <string>

namespace GELib
{

class CCharacter;
class CCharacterDesc;
class CAnimationDesc;
class CFx;
class CIOStream;
class CActRecorder;
class CActPlayer;
class CBaseAsset;
class IAttachable;

const unsigned short ACT_OP_MAGIC = 1234;

enum EActOp
{
	OP_NONE = 0,
	OP_ACT_STOP,
	OP_BEGIN_FRAME,
	OP_END_FRAME,
	OP_CREATE_CHARACTER,
	OP_DELETE_CHARACTER,
	OP_UPDATE_CHARACTER,
	OP_CREATE_FX,
	OP_DELETE_FX,
	OP_UPDATE_FX,
	OP_UPDATE_CAMERA
};

class CActBase
{
public:
	CActBase();

	unsigned short AppendString(const std::string &s);
	unsigned short AppendString(CBaseAsset *a);
	const std::string& GetString(unsigned short id);

	bool IsMatrixEqual(const CMatrix &m0, const CMatrix &m1);
	void WriteMatrix(CIOStream &stream, const CMatrix &m0, const CMatrix &m);
	void ReadMatrix(CIOStream &stream, const CMatrix &m0, CMatrix &m);
	bool IsColorEqual(const CVector3 &c0, const CVector3 &c1);
	void WriteColor(CIOStream &stream, const CVector3 &c);
	void ReadColor(CIOStream &stream, CVector3 &c);
	void WriteUShort(CIOStream &stream, unsigned short i);
	void ReadUShort(CIOStream &stream, unsigned short &i);

	int m_iFrameCounter;

	union
	{
		CActPlayer *m_pPlayer;
		CActRecorder *m_pRecorder;
	};
};


class CActFrame : public CActBase
{
public:
	CActFrame();

	void WriteUpdateCommand(CIOStream &stream);
	void ReadUpdateCommand(CIOStream &stream);

	float m_fTime;
};

class CActCharacter : public CActBase
{
public:
	CActCharacter();

	void WriteCreateCommand(CIOStream &stream);
	void ReadCreateCommand(CIOStream &stream);
	void WriteDeleteCommand(CIOStream &stream);
	void ReadDeleteCommand(CIOStream &stream);
	void WriteUpdateCommand(CIOStream &stream);
	void ReadUpdateCommand(CIOStream &stream);

	unsigned short m_iId;
	CCharacter *m_pCharacter;
	CCharacterDesc *m_pDesc;
	CAnimationDesc *m_pAnimation;
	bool m_bShow;
	float m_fAnimSpeed;
	CVector3 m_Color;
	CMatrix m_Matrix;
};

class CActFx : public CActBase
{
public:
	CActFx();

	void WriteCreateCommand(CIOStream &stream);
	void ReadCreateCommand(CIOStream &stream);
	void WriteDeleteCommand(CIOStream &stream);
	void ReadDeleteCommand(CIOStream &stream);
	void WriteUpdateCommand(CIOStream &stream);
	void ReadUpdateCommand(CIOStream &stream);

	unsigned short m_iId;
	CFx *m_pFx;
	unsigned short m_iNameId;
	IAttachable *m_pAttachable[2];
	bool m_bShow;
	bool m_bLoop;
	bool m_bHalted;
	CVector3 m_Color;
	CVector4 m_MaterialParameter;
	float m_fScale;
	float m_fSpeed;
	CMatrix m_Matrix;
};

class CActCamera : public CActBase
{
public:
	CActCamera();
	void Reset();

	void WriteUpdateCommand(CIOStream &stream);
	void ReadUpdateCommand(CIOStream &stream);

	CMatrix m_Matrix;
	float m_fFov;
	float m_fNear;
	float m_fFar;
};


}//GELib

#endif//__GELIB_EXT_ACT_H__

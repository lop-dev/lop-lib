//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Character/AnimationAsset.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_CHARACTER_ANIMATIONASSET_H__
#define __GELIB_CHARACTER_ANIMATIONASSET_H__

#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <GELib/Math/GeMath.h>
#include <GELib/Ext/BaseAsset.h>


namespace GELib
{

const float KEY_TIME_COMPRESS1_MULTIPLIER = 120.0f;
const float KEY_TIME_COMPRESS1_MULTIPLIER_R = 1.0f / KEY_TIME_COMPRESS1_MULTIPLIER;
const float KEY_TIME_COMPRESS2_MULTIPLIER = 120.0f;
const float KEY_TIME_COMPRESS2_MULTIPLIER_R = 1.0f / KEY_TIME_COMPRESS2_MULTIPLIER;

const float KEY_ROTATION_COMPRESS1_MULTIPLIER = 32766.0f;
const float KEY_ROTATION_COMPRESS1_MULTIPLIER_R = 1.0f / KEY_ROTATION_COMPRESS1_MULTIPLIER;
const float KEY_ROTATION_COMPRESS2_MULTIPLIER = 723.0f;
const float KEY_ROTATION_COMPRESS2_MULTIPLIER_R = 1.0f / KEY_ROTATION_COMPRESS2_MULTIPLIER;

const float KEY_TRANSLATION_COMPRESS1_MULTIPLIER = 100.0f;
const float KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R = 1.0f / KEY_TRANSLATION_COMPRESS1_MULTIPLIER;
const float KEY_TRANSLATION_COMPRESS2_MULTIPLIER = 100.0f;
const float KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R = 1.0f / KEY_TRANSLATION_COMPRESS2_MULTIPLIER;

const float KEY_SCALE_COMPRESS1_MULTIPLIER = 1000.0f;
const float KEY_SCALE_COMPRESS1_MULTIPLIER_R = 1.0f / KEY_SCALE_COMPRESS1_MULTIPLIER;
const float KEY_SCALE_COMPRESS2_MULTIPLIER = 63.0f;
const float KEY_SCALE_COMPRESS2_MULTIPLIER_R = 1.0f / KEY_SCALE_COMPRESS2_MULTIPLIER;

class CIOStream;
class CColladaFile;
class CAnimationManager;
class CAnimationAsset;
class CRawTrack;

class CSkeletonAnimationBinding
{
public:
	CAnimationAsset *m_pAnimation;
    std::vector<short> m_TrackIndices;	// mapping from skeleton bone index to animation track index
};

const int ASSETCLASS_ANIMATION = 8;

class CAnimationAsset : public CBaseAsset
{
public:
    CAnimationAsset(CAnimationManager *manager, const char *cacheKey=0);
    virtual ~CAnimationAsset();

	virtual int GetClassId() { return ASSETCLASS_ANIMATION; }
	CAnimationManager* GetManager() { return m_pManager; }
    virtual void ManagerDelete();

    bool Save(CIOStream &stream);
    bool Load(CIOStream &stream, const CLoadParameter &param);
    bool Import(CColladaFile &collada, int compressLevel);
    bool ImportAdditive(CColladaFile &collada, CColladaFile &baseCollada, int compressLevel);
    void Unload();

    float GetDuration() { return m_fDuration; }
	bool IsAdditive() { return m_bAdditive; }

    CSkeletonAnimationBinding* GetSkeletonAnimationBinding(CAnimationAsset *animation);

    typedef char Key;

    enum EChannel
    {
        CHANNEL_ROTATION = 1 << 0,
        CHANNEL_TRANSLATION = 1 << 1,
        CHANNEL_SCALE = 1 << 2
    };

    // compress level
    // 0: time: 4 bytes, rotation: 16 bytes, translation: 12 bytes, scale: 12 bytes
    // 1: time: 2 bytes, rotation: 8 bytes, translation: 6 bytes, scale: 6 bytes
    // 2: time: 1 bytes, rotation: 4 bytes, translation: 3 bytes, scale: 3 bytes

    class CTrack
    {
    public:
        CTrack();
        void Compress(CRawTrack &rawTrack, Key *key, char timeCompress, char rotCompress, char transCompress, char scaleCompress);
        void SampleKey(float time, int &keyIndexHint, CQuaternion &rotation, CVector3 &translation, CVector3 &scale);

        std::string m_Name;
        CMatrix m_LocalMatrix;
        char m_iTimeCompressLevel;
        char m_iRotationCompressLevel;
        char m_iTranslationCompressLevel;
        char m_iScaleCompressLevel;
        short m_iChannels;
        int m_iNumKeys;
        CTrack *m_pParent;

        // computed after loading, not saved
        void ComputeChannelOffset();
        CQuaternion m_InitRotation;
        CVector3 m_InitTranslation;
        CVector3 m_InitScale;
        char m_iKeySize;
        char m_iRotateOffset;
        char m_iTranslateOffset;
        char m_iScaleOffset;

        /* key format
        float time;
        CQuaternion rotation;  // optional
        CVector3 translation;  // optional
        CVector3 scale;        // optional
        */
        Key *m_pKeys;

        Key* GetKey(int index) { return m_pKeys + index * m_iKeySize; }
        void GetKeysFromTime(float time, int &keyIndexHint, Key *&key0, Key *&key1);

        float GetKeyTime(Key *key);
        CQuaternion GetKeyRotation(Key *key);
        CVector3 GetKeyTranslation(Key *key);
        CVector3 GetKeyScale(Key *key);
        CQuaternion SampleKeyRotation(Key *key0, Key *key1, float t);
        CVector3 SampleKeyTranslation(Key *key0, Key *key1, float t);
        CVector3 SampleKeyScale(Key *key0, Key *key1, float t);

	private:

		static int CompressQuaternion(const CQuaternion &q);
		static CQuaternion DecompressQuaternion(int q);
    };

    int GetNumTracks() { return (int)m_Tracks.size(); }
    CTrack* GetTrack(int index) { return &m_Tracks[index]; }
    CTrack* GetTrackByName(const char *name);
    int GetTrackIndexByName(const char *name);

private:

    float m_fDuration;
	bool m_bAdditive;
    std::vector<CTrack> m_Tracks;
    CAnimationManager *m_pManager;
	Key *m_pAllKeys;

	typedef std::map<int, CSkeletonAnimationBinding> SkeletonAnimationBindingCache;
    SkeletonAnimationBindingCache m_SkeletonAnimationBindingCache;
};

inline float CAnimationAsset::CTrack::GetKeyTime(Key *key)
{
    switch (m_iTimeCompressLevel)
	{
	case 0:
		{
			float *k = (float*)key;
			return *k;
		}
	case 1:
		{
			unsigned short *k = (unsigned short*)key;
			return float(*k * KEY_TIME_COMPRESS1_MULTIPLIER_R);
		}
	case 2:
		{
			unsigned char *k = (unsigned char*)key;
			return float(*k * KEY_TIME_COMPRESS2_MULTIPLIER_R);
		}
	default:
		assert(false);
		return 0;
	}
}

inline CQuaternion CAnimationAsset::CTrack::GetKeyRotation(Key *key)
{
    switch (m_iRotationCompressLevel)
	{
	case 0:
        return *(CQuaternion*)(key + m_iRotateOffset);
	case 1:
		{
			short *k = (short*)(key + m_iRotateOffset);
			return CQuaternion(k[0] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R, k[1] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R,
							   k[2] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R, k[3] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R);
		}
	case 2:
		{
			return DecompressQuaternion(*(int*)(key + m_iRotateOffset));
		}
	default:
		assert(false);
		return CQuaternion(0, 0, 0, 1);
	}
}

inline CVector3 CAnimationAsset::CTrack::GetKeyTranslation(Key *key)
{
    switch (m_iTranslationCompressLevel)
	{
	case 0:
        return *(CVector3*)(key + m_iTranslateOffset);
	case 1:
		{
			short *k = (short*)(key + m_iTranslateOffset);
			return CVector3(k[0] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R, k[1] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R,
							k[2] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R);
		}
	case 2:
		{
			char *k = (char*)(key + m_iTranslateOffset);
			return CVector3(k[0] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R, k[1] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R,
							k[2] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R);
		}
	default:
		assert(false);
		return CVector3(0, 0, 0);
	}
}

inline CVector3 CAnimationAsset::CTrack::GetKeyScale(Key *key)
{
    switch (m_iScaleCompressLevel)
	{
	case 0:
        return *(CVector3*)(key + m_iScaleOffset);
	case 1:
		{
			unsigned short *k = (unsigned short*)(key + m_iScaleOffset);
			return CVector3(k[0] * KEY_SCALE_COMPRESS1_MULTIPLIER_R, k[1] * KEY_SCALE_COMPRESS1_MULTIPLIER_R,
							k[2] * KEY_SCALE_COMPRESS1_MULTIPLIER_R);
		}
	case 2:
		{
			unsigned char *k = (unsigned char*)(key + m_iScaleOffset);
			return CVector3(k[0] * KEY_SCALE_COMPRESS2_MULTIPLIER_R, k[1] * KEY_SCALE_COMPRESS2_MULTIPLIER_R,
							k[2] * KEY_SCALE_COMPRESS2_MULTIPLIER_R);
		}
	default:
		assert(false);
		return CVector3(1, 1, 1);
	}

}

inline CQuaternion CAnimationAsset::CTrack::SampleKeyRotation(Key *key0, Key *key1, float t)
{
    switch (m_iRotationCompressLevel)
	{
	case 0:
        return Nlerp(*(CQuaternion*)(key0 + m_iRotateOffset), *(CQuaternion*)(key1 + m_iRotateOffset), t);
	case 1:
		{
			short *k0 = (short*)(key0 + m_iRotateOffset);
			short *k1 = (short*)(key1 + m_iRotateOffset);
			return Nlerp(CQuaternion(k0[0] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R, k0[1] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R,
									 k0[2] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R, k0[3] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R),
						 CQuaternion(k1[0] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R, k1[1] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R,
									 k1[2] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R, k1[3] * KEY_ROTATION_COMPRESS1_MULTIPLIER_R), t);
		}
	case 2:
		{
			return Nlerp(DecompressQuaternion(*(int*)(key0 + m_iRotateOffset)), DecompressQuaternion(*(int*)(key1 + m_iRotateOffset)), t);
		}
	default:
		assert(false);
		return CQuaternion(0, 0, 0, 1);
	}
}

inline CVector3 CAnimationAsset::CTrack::SampleKeyTranslation(Key *key0, Key *key1, float t)
{
    switch (m_iTranslationCompressLevel)
	{
	case 0:
        return Lerp(*(CVector3*)(key0 + m_iTranslateOffset), *(CVector3*)(key1 + m_iTranslateOffset), t);
	case 1:
		{
			short *k0 = (short*)(key0 + m_iTranslateOffset);
			short *k1 = (short*)(key1 + m_iTranslateOffset);
			return Lerp(CVector3(k0[0] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R, k0[1] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R,
								 k0[2] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R),
						CVector3(k1[0] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R, k1[1] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R,
								 k1[2] * KEY_TRANSLATION_COMPRESS1_MULTIPLIER_R), t);
		}
	case 2:
		{
			char *k0 = (char*)(key0 + m_iTranslateOffset);
			char *k1 = (char*)(key1 + m_iTranslateOffset);
			return Lerp(CVector3(k0[0] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R, k0[1] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R,
								 k0[2] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R),
						CVector3(k1[0] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R, k1[1] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R,
								 k1[2] * KEY_TRANSLATION_COMPRESS2_MULTIPLIER_R), t);
		}
	default:
		assert(false);
		return CVector3(0, 0, 0);
	}
}

inline CVector3 CAnimationAsset::CTrack::SampleKeyScale(Key *key0, Key *key1, float t)
{
    switch (m_iScaleCompressLevel)
	{
	case 0:
        return Lerp(*(CVector3*)(key0 + m_iScaleOffset), *(CVector3*)(key1 + m_iScaleOffset), t);
	case 1:
		{
			unsigned short *k0 = (unsigned short*)(key0 + m_iScaleOffset);
			unsigned short *k1 = (unsigned short*)(key1 + m_iScaleOffset);
			return Lerp(CVector3(k0[0] * KEY_SCALE_COMPRESS1_MULTIPLIER_R, k0[1] * KEY_SCALE_COMPRESS1_MULTIPLIER_R,
								 k0[2] * KEY_SCALE_COMPRESS1_MULTIPLIER_R),
						CVector3(k1[0] * KEY_SCALE_COMPRESS1_MULTIPLIER_R, k1[1] * KEY_SCALE_COMPRESS1_MULTIPLIER_R,
								 k1[2] * KEY_SCALE_COMPRESS1_MULTIPLIER_R), t);
		}
	case 2:
		{
			unsigned char *k0 = (unsigned char*)(key0 + m_iScaleOffset);
			unsigned char *k1 = (unsigned char*)(key1 + m_iScaleOffset);
			return Lerp(CVector3(k0[0] * KEY_SCALE_COMPRESS2_MULTIPLIER_R, k0[1] * KEY_SCALE_COMPRESS2_MULTIPLIER_R,
								 k0[2] * KEY_SCALE_COMPRESS2_MULTIPLIER_R),
						CVector3(k1[0] * KEY_SCALE_COMPRESS2_MULTIPLIER_R, k1[1] * KEY_SCALE_COMPRESS2_MULTIPLIER_R,
								 k1[2] * KEY_SCALE_COMPRESS2_MULTIPLIER_R), t);
		}
	default:
		assert(false);
		return CVector3(1, 1, 1);
	}
}


}//GELib

#endif//__GELIB_CHARACTER_ANIMATIONASSET_H__

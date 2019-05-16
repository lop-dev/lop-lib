//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/Utility/DataBuffer.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_UTILITY_DATABUFFER_H__
#define __GELIB_UTILITY_DATABUFFER_H__


namespace GELib
{

class CDataBuffer
{
public:
	CDataBuffer();
	~CDataBuffer();

	unsigned char * Allocate(int size);
	void Free();

	unsigned char * GetData() { return m_pData; }
	int GetSize() {	return m_iSize;  }
	bool IsEmpty() { return m_iSize == 0; }

private:
	unsigned char *m_pData;
	int m_iSize;
};


}//GELib

#endif//__GELIB_UTILITY_DATABUFFER_H__

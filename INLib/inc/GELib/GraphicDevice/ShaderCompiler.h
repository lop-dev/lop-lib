//////////////////////////////////////////////////////////////////////
//  created:    2010/06/06
//  filename:   GELib/GraphicDevice/ShaderCompiler.h
//  author:     League of Perfect
/// @brief
///
//////////////////////////////////////////////////////////////////////
#ifndef __GELIB_GRAPHICDEVICE_SHADERCOMPILER_H__
#define __GELIB_GRAPHICDEVICE_SHADERCOMPILER_H__

#include <string>
#include <vector>
#include <GELib/GeConfig.h>

namespace GELib
{

class CShaderCompiler
{
public:
	GELIB_API CShaderCompiler();
	GELIB_API virtual ~CShaderCompiler();

	enum EOption
	{
		ENABLE_DEBUG = 1<<0,
		SKIP_OPTIMIZATION = 1<<1,
		AVOID_FLOW_CONTROL = 1<<2
	};
	GELIB_API void SetOptions(int option);

	GELIB_API bool CompileFile(const char *fileName, const char *funcName, const char *profile, char const * const *defines = 0);
	GELIB_API bool CompileText(const char *srcCode, const char *funcName, const char *profile, char const * const *defines = 0, int textLen=-1);
	GELIB_API bool AssembleFile(const char *fileName);
	GELIB_API bool AssembleText(const char *assembly, int textLen=-1);
	GELIB_API const char * Disassemble(const void *code);
	GELIB_API const char * PreprocessText(const char *srcCode, char const * const *define = 0);

	GELIB_API bool gl_OptimizeShaderCode(const char *code, bool isVertex, std::string &optCode);

	GELIB_API void* GetCode();
	GELIB_API int GetCodeSize();

	class Constant
	{
	public:
		std::string m_Name;
		short m_iRegisterIndex;
	    short m_iRegisterCount;
	};

	GELIB_API int GetNumConstants();
	GELIB_API const Constant* GetConstant(int idx) const;
	GELIB_API const Constant* GetConstantByName(const char *name) const;

	GELIB_API const char * GetErrorMessage() const;

private:

	void ProcessCompileResult(void *code, void *errMsg, void *constantTable);

	int m_iOptions;
	std::string m_ErrorMessage;
	std::string m_TempCode;
	std::vector<char> m_Code;
	std::vector<Constant> m_ConstantTable;
};



}//GELib

#endif//__GELIB_GRAPHICDEVICE_SHADERCOMPILER_H__

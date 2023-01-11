#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CString.h"

class CShader
{
public:

	CString* m_filename;
	CString* m_version;
	CString* m_entryPoint;

	ID3DBlob* m_shader;


	CShader();

	CShader(CErrorLog* errorLog, const char* shaderName, const char* entryPoint, const char* version);

	~CShader();

private:

	CErrorLog* m_errorLog;

	ID3DBlob* m_errors;
};
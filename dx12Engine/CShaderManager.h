#pragma once

#include "standard.h"

#include "CErrorLog.h"
#include "CLinkList.h"
#include "CShader.h"

class CShaderManager
{
public:

	CLinkList<CShader>* m_shaders;


	CShaderManager();

	CShaderManager(CErrorLog* errorLog);

	~CShaderManager();


	CShader* Create(const char* shaderName, const char* entryPoint, const char* version);

	CShader* Get(const char* shaderName);

	void Delete(const char* shaderName);

private:

	CErrorLog* m_errorLog;
};
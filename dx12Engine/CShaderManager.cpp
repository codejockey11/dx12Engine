#include "CShaderManager.h"

CShaderManager::CShaderManager()
{
	memset(this, 0x00, sizeof(CShaderManager));
}

CShaderManager::CShaderManager(CErrorLog* errorLog)
{
	memset(this, 0x00, sizeof(CShaderManager));

	m_errorLog = errorLog;

	m_shaders = new CLinkList<CShader>();
}

CShaderManager::~CShaderManager()
{
	delete m_shaders;
}

CShader* CShaderManager::Create(const char* shaderName, const char* entryPoint, const char* version)
{
	CShader* shader = CShaderManager::Get(shaderName);

	if (shader)
	{
		return shader;
	}

	shader = new CShader(m_errorLog, shaderName, entryPoint, version);

	if (shader->m_shader)
	{
		m_shaders->Add(shader, shader->m_filename->GetText());

		return shader;
	}

	m_errorLog->WriteError("CShaderManager::MakeShader:%s\n", shaderName);

	delete shader;

	return nullptr;
}

CShader* CShaderManager::Get(const char* shaderName)
{
	CLinkListNode<CShader>* lln = m_shaders->Search(shaderName);

	if (lln)
	{
		return lln->m_element;
	}

	return nullptr;
}

void CShaderManager::Delete(const char* shaderName)
{
	CLinkListNode<CShader>* lln = m_shaders->Search(shaderName);

	if (lln)
	{
		m_shaders->Delete(lln);
	}
}
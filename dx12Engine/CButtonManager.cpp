#include "CButtonManager.h"

/*
*/
CButtonManager::CButtonManager()
{
	memset(this, 0x00, sizeof(CButtonManager));
}

/*
*/
CButtonManager::CButtonManager(CGlobalObjects* globalObjects)
{
	memset(this, 0x00, sizeof(CButtonManager));

	m_globalObjects = globalObjects;

	m_buttons = new CLinkList<CButton>();
}

/*
*/
CButtonManager::~CButtonManager()
{
	delete m_buttons;
}

/*
*/
CButton* CButtonManager::Create(HMENU hMenu, const char* name, const char* text,
	CTexture* image, const char* font, float fontSize, CVertex2 size, CVertex2 position, BYTE audience, BYTE type)
{
	CButton* button = CButtonManager::Get(name);

	if (button != nullptr)
	{
		return button;
	}

	button = new CButton(m_globalObjects, hMenu, name, text, image, font, fontSize, size, position, audience, type);

	if (button->m_isInitialized)
	{
		m_buttons->Add(button, name);

		return button;
	}

	delete button;

	m_globalObjects->m_errorLog->WriteError("CButtonManager::MakeButton:%s\n", name);

	return nullptr;
}

/*
*/
void CButtonManager::Delete(const char* name)
{
	CLinkListNode<CButton>* lln = m_buttons->Search(name);

	if (lln)
	{
		m_buttons->Delete(lln);
	}
}

/*
*/
CButton* CButtonManager::Get(const char* name)
{
	CLinkListNode<CButton>* lln = m_buttons->Search(name);

	if (lln)
	{
		return (CButton*)lln->m_object;
	}

	return nullptr;
}
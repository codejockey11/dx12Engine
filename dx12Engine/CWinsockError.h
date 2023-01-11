#pragma once

#include "standard.h"

#include "CLinkList.h"
#include "CWinsockErrorItem.h"

class CWinsockError
{
public:

	CLinkList<CWinsockErrorItem>* m_errors;


	CWinsockError();

	~CWinsockError();


	CWinsockErrorItem* GetError(UINT e);

	void AddItem(CWinsockErrorItem* i);
};
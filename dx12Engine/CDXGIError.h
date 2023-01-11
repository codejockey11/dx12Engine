#pragma once

#include "standard.h"

#include "CLinkList.h"
#include "CDXGIErrorItem.h"

class CDXGIError
{
public:

	CLinkList<CDXGIErrorItem>* m_errors;

	CDXGIErrorItem* m_defaultError;


	CDXGIError();

	~CDXGIError();


	CDXGIErrorItem* GetError(UINT error);

	void AddItem(CDXGIErrorItem* item);
};
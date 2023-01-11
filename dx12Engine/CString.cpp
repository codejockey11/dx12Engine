#include "CString.h"

CString::CString()
{
	memset(this, 0x00, sizeof(CString));
}

CString::CString(const char* string)
{
	memset(this, 0x00, sizeof(CString));

	if (string == nullptr)
	{
		return;
	}

	m_length = (UINT)strlen(string);

	m_text = new char[(size_t)m_length + 1];

	memset((void*)m_text, 0x00, (size_t)m_length + 1);

	memcpy((void*)m_text, (void*)string, m_length);
}

CString::CString(UINT length)
{
	memset(this, 0x00, sizeof(CString));

	m_length = length;

	m_text = new char[(size_t)m_length + 1];

	memset((void*)m_text, 0x00, (size_t)m_length + 1);
}

CString::~CString()
{
	delete[] m_text;
}

void CString::Clear()
{
	memset((void*)m_text, 0x00, m_length);
}

void CString::Format(const char* format, ...)
{
	va_list	argptr;

	va_start(argptr, format);

	Clear();

	vsprintf_s(m_text, m_length, format, argptr);

	va_end(argptr);
}

void CString::Concat(const char* c)
{
	UINT tempLength = m_length + (UINT)strlen(c);

	char* tempText = new char[tempLength + 1];

	memset((void*)tempText, 0x00, (size_t)tempLength + 1);

	memcpy((void*)tempText, (void*)m_text, m_length);

	memcpy((void*)&tempText[m_length], (void*)c, (size_t)strlen(c));

	delete[] m_text;

	m_text = tempText;

	m_length = tempLength;
}

bool CString::Search(const char* c)
{
	size_t cl = strlen(c);

	int count = 0;

	for (UINT i = 0; i < m_length; i++)
	{
		count = 0;

		for (size_t ii = 0; ii < cl; ii++)
		{
			if ((i + ii) >= m_length)
			{
				break;
			}

			if (m_text[i + ii] == c[ii])
			{
				count++;
			}
			else
			{
				break;
			}
		}

		if (count == cl)
		{
			return true;
		}
	}

	return false;
}
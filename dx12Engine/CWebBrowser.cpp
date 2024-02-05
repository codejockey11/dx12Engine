#include "CWebBrowser.h"

CWebBrowser::CWebBrowser()
{
	memset(this, 0x00, sizeof(CWebBrowser));
}

CWebBrowser::CWebBrowser(HWND hWndParent, int left, int top, int right, int bottom)
{
	m_iComRefCount = 0;
	::SetRect(&m_rObject, left, top, right, bottom);
	m_hWndParent = hWndParent;

	if (CreateBrowser() == FALSE)
	{
		return;
	}

	ShowWindow(GetControlWindow(), SW_SHOW);

	this->Navigate("about:blank");
}

CWebBrowser::~CWebBrowser()
{
}

bool CWebBrowser::CreateBrowser()
{
	HRESULT hr;
	hr = ::OleCreate(CLSID_WebBrowser,
		IID_IOleObject, OLERENDER_DRAW, 0, this, this,
		(void**)&m_oleObject);

	if (FAILED(hr))
	{
		MessageBox(NULL, _T("Cannot create oleObject CLSID_CWebBrowser"),
			_T("Error"),
			MB_ICONERROR);
		return FALSE;
	}

	hr = m_oleObject->SetClientSite(this);
	hr = OleSetContainedObject(m_oleObject, TRUE);

	RECT posRect;
	::SetRect(&posRect, -300, -300, 300, 300);
	hr = m_oleObject->DoVerb(OLEIVERB_INPLACEACTIVATE,
		NULL, this, -1, m_hWndParent, &posRect);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("oleObject->DoVerb() failed"),
			_T("Error"),
			MB_ICONERROR);
		return FALSE;
	}

	hr = m_oleObject->QueryInterface(&m_webBrowser2);
	if (FAILED(hr))
	{
		MessageBox(NULL, _T("oleObject->QueryInterface(&webBrowser2) failed"),
			_T("Error"),
			MB_ICONERROR);
		return FALSE;
	}

	this->m_webBrowser2->put_Silent(VARIANT_TRUE);

	return TRUE;
}

RECT CWebBrowser::PixelToHiMetric(const RECT& _rc)
{
	static bool s_initialized = false;
	static int s_pixelsPerInchX, s_pixelsPerInchY;
	if (!s_initialized)
	{
		HDC hdc = ::GetDC(0);
		s_pixelsPerInchX = ::GetDeviceCaps(hdc, LOGPIXELSX);
		s_pixelsPerInchY = ::GetDeviceCaps(hdc, LOGPIXELSY);
		::ReleaseDC(0, hdc);
		s_initialized = true;
	}

	RECT rc = {};
	rc.left = MulDiv(2540, _rc.left, s_pixelsPerInchX);
	rc.top = MulDiv(2540, _rc.top, s_pixelsPerInchY);
	rc.right = MulDiv(2540, _rc.right, s_pixelsPerInchX);
	rc.bottom = MulDiv(2540, _rc.bottom, s_pixelsPerInchY);
	return rc;
}

void CWebBrowser::SetRect(const RECT& _rc)
{
	m_rObject = _rc;

	{
		RECT hiMetricRect = PixelToHiMetric(m_rObject);
		SIZEL sz = {};
		sz.cx = hiMetricRect.right - hiMetricRect.left;
		sz.cy = hiMetricRect.bottom - hiMetricRect.top;
		m_oleObject->SetExtent(DVASPECT_CONTENT, &sz);
	}

	if (m_oleInPlaceObject != 0)
	{
		m_oleInPlaceObject->SetObjectRects(&m_rObject, &m_rObject);
	}
}

// ----- Control methods -----

void CWebBrowser::GoBack()
{
	this->m_webBrowser2->GoBack();
}

void CWebBrowser::GoForward()
{
	this->m_webBrowser2->GoForward();
}

void CWebBrowser::Refresh()
{
	this->m_webBrowser2->Refresh();
}

void CWebBrowser::Navigate(const char* szUrl)
{
	bstr_t url(szUrl);
	variant_t flags(0x02u); //navNoHistory
	this->m_webBrowser2->Navigate(url, &flags, 0, 0, 0);
}

// ----- IUnknown -----

HRESULT STDMETHODCALLTYPE CWebBrowser::QueryInterface(REFIID riid,
	void** ppvObject)
{
	if (riid == __uuidof(IUnknown))
	{
		(*ppvObject) = static_cast<IOleClientSite*>(this);
	}
	else if (riid == __uuidof(IOleInPlaceSite))
	{
		(*ppvObject) = static_cast<IOleInPlaceSite*>(this);
	}
	else
	{
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CWebBrowser::AddRef(void)
{
	m_iComRefCount++;
	return m_iComRefCount;
}

ULONG STDMETHODCALLTYPE CWebBrowser::Release(void)
{
	m_iComRefCount--;
	return m_iComRefCount;
}

// ---------- IOleWindow ----------

HRESULT STDMETHODCALLTYPE CWebBrowser::GetWindow(
	__RPC__deref_out_opt HWND* phwnd)
{
	(*phwnd) = m_hWndParent;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::ContextSensitiveHelp(
	BOOL fEnterMode)
{
	return E_NOTIMPL;
}

// ---------- IOleInPlaceSite ----------

HRESULT STDMETHODCALLTYPE CWebBrowser::CanInPlaceActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::OnInPlaceActivate(void)
{
	OleLockRunning(m_oleObject, TRUE, FALSE);
	m_oleObject->QueryInterface(&m_oleInPlaceObject);
	m_oleInPlaceObject->SetObjectRects(&m_rObject, &m_rObject);

	return S_OK;

}

HRESULT STDMETHODCALLTYPE CWebBrowser::OnUIActivate(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::GetWindowContext(
	__RPC__deref_out_opt IOleInPlaceFrame** ppFrame,
	__RPC__deref_out_opt IOleInPlaceUIWindow** ppDoc,
	__RPC__out LPRECT lprcPosRect,
	__RPC__out LPRECT lprcClipRect,
	__RPC__inout LPOLEINPLACEFRAMEINFO lpFrameInfo)
{
	HWND hwnd = m_hWndParent;

	(*ppFrame) = NULL;
	(*ppDoc) = NULL;
	(*lprcPosRect).left = m_rObject.left;
	(*lprcPosRect).top = m_rObject.top;
	(*lprcPosRect).right = m_rObject.right;
	(*lprcPosRect).bottom = m_rObject.bottom;
	*lprcClipRect = *lprcPosRect;

	lpFrameInfo->fMDIApp = false;
	lpFrameInfo->hwndFrame = hwnd;
	lpFrameInfo->haccel = NULL;
	lpFrameInfo->cAccelEntries = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::Scroll(
	SIZE scrollExtant)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::OnUIDeactivate(
	BOOL fUndoable)
{
	return S_OK;
}

HWND CWebBrowser::GetControlWindow()
{
	if (m_hWndControl != 0)
		return m_hWndControl;

	if (m_oleInPlaceObject == 0)
		return 0;

	m_oleInPlaceObject->GetWindow(&m_hWndControl);
	return m_hWndControl;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::OnInPlaceDeactivate(void)
{
	m_hWndControl = 0;
	m_oleInPlaceObject = 0;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::DiscardUndoState(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::DeactivateAndUndo(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::OnPosRectChange(
	__RPC__in LPCRECT lprcPosRect)
{
	return E_NOTIMPL;
}

// ---------- IOleClientSite ----------

HRESULT STDMETHODCALLTYPE CWebBrowser::SaveObject(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::GetMoniker(
	DWORD dwAssign,
	DWORD dwWhichMoniker,
	__RPC__deref_out_opt IMoniker** ppmk)
{
	if ((dwAssign == OLEGETMONIKER_ONLYIFTHERE) &&
		(dwWhichMoniker == OLEWHICHMK_CONTAINER))
		return E_FAIL;

	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::GetContainer(
	__RPC__deref_out_opt IOleContainer** ppContainer)
{
	return E_NOINTERFACE;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::ShowObject(void)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::OnShowWindow(
	BOOL fShow)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::RequestNewObjectLayout(void)
{
	return E_NOTIMPL;
}

// ----- IStorage -----

HRESULT STDMETHODCALLTYPE CWebBrowser::CreateStream(
	__RPC__in_string const OLECHAR* pwcsName,
	DWORD grfMode,
	DWORD reserved1,
	DWORD reserved2,
	__RPC__deref_out_opt IStream** ppstm)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::OpenStream(
	const OLECHAR* pwcsName,
	void* reserved1,
	DWORD grfMode,
	DWORD reserved2,
	IStream** ppstm)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::CreateStorage(
	__RPC__in_string const OLECHAR* pwcsName,
	DWORD grfMode,
	DWORD reserved1,
	DWORD reserved2,
	__RPC__deref_out_opt IStorage** ppstg)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::OpenStorage(
	__RPC__in_opt_string const OLECHAR* pwcsName,
	__RPC__in_opt IStorage* pstgPriority,
	DWORD grfMode,
	__RPC__deref_opt_in_opt SNB snbExclude,
	DWORD reserved,
	__RPC__deref_out_opt IStorage** ppstg)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::CopyTo(
	DWORD ciidExclude,
	const IID* rgiidExclude,
	__RPC__in_opt  SNB snbExclude,
	IStorage* pstgDest)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::MoveElementTo(
	__RPC__in_string const OLECHAR* pwcsName,
	__RPC__in_opt IStorage* pstgDest,
	__RPC__in_string const OLECHAR* pwcsNewName,
	DWORD grfFlags)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::Commit(
	DWORD grfCommitFlags)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::Revert(void)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::EnumElements(
	DWORD reserved1,
	void* reserved2,
	DWORD reserved3,
	IEnumSTATSTG** ppenum)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::DestroyElement(
	__RPC__in_string const OLECHAR* pwcsName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::RenameElement(
	__RPC__in_string const OLECHAR* pwcsOldName,
	__RPC__in_string const OLECHAR* pwcsNewName)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::SetElementTimes(
	__RPC__in_opt_string const OLECHAR* pwcsName,
	__RPC__in_opt const FILETIME* pctime,
	__RPC__in_opt const FILETIME* patime,
	__RPC__in_opt const FILETIME* pmtime)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::SetClass(
	__RPC__in REFCLSID clsid)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::SetStateBits(
	DWORD grfStateBits,
	DWORD grfMask)
{
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CWebBrowser::Stat(
	__RPC__out STATSTG* pstatstg,
	DWORD grfStatFlag)
{
	return E_NOTIMPL;
}
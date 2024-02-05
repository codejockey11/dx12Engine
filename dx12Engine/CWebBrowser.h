#pragma once

#include "standard.h"

// Must be in a child window of its own
// Must use OleInitialize(NULL);
// OleUninitialize();
// OleUninitialize calls the CoUninitialize function internally
// HKEY_CURRENT_USER\SOFTWARE\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_BROWSER_EMULATION "dx12Engine.exe" = dword:0x00002AF9
// You must set a registry key for the version of IE that you want to use
// https://learn.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/general-info/ee330730(v=vs.85)?redirectedfrom=MSDN
/*
	webBrowser = new CWebBrowser(globalObjects->window->hWnd);

	RECT rc;
	rc.left = 0;
	rc.top = 0;
	rc.right = 400;
	rc.bottom = 400;
	webBrowser->SetRect(rc);

	webBrowser->Navigate("http://www.google.com");

*/
// from: https://www.codeproject.com/Tips/800585/Embedded-WebBrowser-IWebBrowser-in-Pure-Cplusplus

class CWebBrowser :
	public IOleClientSite,
	public IOleInPlaceSite,
	public IStorage
{

public:

	CWebBrowser();

	CWebBrowser(HWND hWndParent, int left, int top, int right, int bottom);

	~CWebBrowser();

	bool CreateBrowser();

	RECT PixelToHiMetric(const RECT& _rc);

	virtual void SetRect(const RECT& _rc);

	// ----- Control methods -----

	void GoBack();

	void GoForward();

	void Refresh();

	void Navigate(const char* szUrl);

	// ----- IUnknown -----

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,
		void** ppvObject) override;

	virtual ULONG STDMETHODCALLTYPE AddRef(void);

	virtual ULONG STDMETHODCALLTYPE Release(void);

	// ---------- IOleWindow ----------

	virtual HRESULT STDMETHODCALLTYPE GetWindow(
		__RPC__deref_out_opt HWND* phwnd) override;

	virtual HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(
		BOOL fEnterMode) override;

	// ---------- IOleInPlaceSite ----------

	virtual HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE OnUIActivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE GetWindowContext(
		__RPC__deref_out_opt IOleInPlaceFrame** ppFrame,
		__RPC__deref_out_opt IOleInPlaceUIWindow** ppDoc,
		__RPC__out LPRECT lprcPosRect,
		__RPC__out LPRECT lprcClipRect,
		__RPC__inout LPOLEINPLACEFRAMEINFO lpFrameInfo) override;

	virtual HRESULT STDMETHODCALLTYPE Scroll(
		SIZE scrollExtant) override;

	virtual HRESULT STDMETHODCALLTYPE OnUIDeactivate(
		BOOL fUndoable) override;

	virtual HWND GetControlWindow();

	virtual HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void) override;

	virtual HRESULT STDMETHODCALLTYPE DiscardUndoState(void) override;

	virtual HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void) override;

	virtual HRESULT STDMETHODCALLTYPE OnPosRectChange(
		__RPC__in LPCRECT lprcPosRect) override;

	// ---------- IOleClientSite ----------

	virtual HRESULT STDMETHODCALLTYPE SaveObject(void) override;

	virtual HRESULT STDMETHODCALLTYPE GetMoniker(
		DWORD dwAssign,
		DWORD dwWhichMoniker,
		__RPC__deref_out_opt IMoniker** ppmk) override;

	virtual HRESULT STDMETHODCALLTYPE GetContainer(
		__RPC__deref_out_opt IOleContainer** ppContainer) override;

	virtual HRESULT STDMETHODCALLTYPE ShowObject(void) override;
	virtual HRESULT STDMETHODCALLTYPE OnShowWindow(
		BOOL fShow) override;

	virtual HRESULT STDMETHODCALLTYPE RequestNewObjectLayout(void) override;

	// ----- IStorage -----

	virtual HRESULT STDMETHODCALLTYPE CreateStream(
		__RPC__in_string const OLECHAR* pwcsName,
		DWORD grfMode,
		DWORD reserved1,
		DWORD reserved2,
		__RPC__deref_out_opt IStream** ppstm) override;

	virtual HRESULT STDMETHODCALLTYPE OpenStream(
		const OLECHAR* pwcsName,
		void* reserved1,
		DWORD grfMode,
		DWORD reserved2,
		IStream** ppstm) override;

	virtual HRESULT STDMETHODCALLTYPE CreateStorage(
		__RPC__in_string const OLECHAR* pwcsName,
		DWORD grfMode,
		DWORD reserved1,
		DWORD reserved2,
		__RPC__deref_out_opt IStorage** ppstg) override;

	virtual HRESULT STDMETHODCALLTYPE OpenStorage(
		__RPC__in_opt_string const OLECHAR* pwcsName,
		__RPC__in_opt IStorage* pstgPriority,
		DWORD grfMode,
		__RPC__deref_opt_in_opt SNB snbExclude,
		DWORD reserved,
		__RPC__deref_out_opt IStorage** ppstg) override;

	virtual HRESULT STDMETHODCALLTYPE CopyTo(
		DWORD ciidExclude,
		const IID* rgiidExclude,
		__RPC__in_opt  SNB snbExclude,
		IStorage* pstgDest) override;

	virtual HRESULT STDMETHODCALLTYPE MoveElementTo(
		__RPC__in_string const OLECHAR* pwcsName,
		__RPC__in_opt IStorage* pstgDest,
		__RPC__in_string const OLECHAR* pwcsNewName,
		DWORD grfFlags) override;

	virtual HRESULT STDMETHODCALLTYPE Commit(
		DWORD grfCommitFlags) override;

	virtual HRESULT STDMETHODCALLTYPE Revert(void) override;

	virtual HRESULT STDMETHODCALLTYPE EnumElements(
		DWORD reserved1,
		void* reserved2,
		DWORD reserved3,
		IEnumSTATSTG** ppenum) override;

	virtual HRESULT STDMETHODCALLTYPE DestroyElement(
		__RPC__in_string const OLECHAR* pwcsName) override;

	virtual HRESULT STDMETHODCALLTYPE RenameElement(
		__RPC__in_string const OLECHAR* pwcsOldName,
		__RPC__in_string const OLECHAR* pwcsNewName) override;

	virtual HRESULT STDMETHODCALLTYPE SetElementTimes(
		__RPC__in_opt_string const OLECHAR* pwcsName,
		__RPC__in_opt const FILETIME* pctime,
		__RPC__in_opt const FILETIME* patime,
		__RPC__in_opt const FILETIME* pmtime) override;

	virtual HRESULT STDMETHODCALLTYPE SetClass(
		__RPC__in REFCLSID clsid) override;
	virtual HRESULT STDMETHODCALLTYPE SetStateBits(
		DWORD grfStateBits,
		DWORD grfMask) override;

	virtual HRESULT STDMETHODCALLTYPE Stat(
		__RPC__out STATSTG* pstatstg,
		DWORD grfStatFlag) override;

protected:

	IOleObject* m_oleObject;
	IOleInPlaceObject* m_oleInPlaceObject;

	IWebBrowser2* m_webBrowser2;

	LONG m_iComRefCount;

	RECT m_rObject;

	HWND m_hWndParent;
	HWND m_hWndControl;
};
// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "..\e2dimpl.h"

e2d::MediaAsyncCallback::MediaAsyncCallback(HWND hwnd, IMFMediaSession * pSession, HANDLE hCloseEvent)
	: m_pSession(pSession)
	, m_hCloseEvent(hCloseEvent)
	, m_hwnd(hwnd)
{
}

HRESULT e2d::MediaAsyncCallback::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(MediaAsyncCallback, IMFAsyncCallback),
		{ 0 }
	};
	return QISearch(this, qit, riid, ppv);
}

ULONG e2d::MediaAsyncCallback::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

ULONG e2d::MediaAsyncCallback::Release()
{
	ULONG uCount = InterlockedDecrement(&m_nRefCount);
	if (uCount == 0)
	{
		delete this;
	}
	return uCount;
}

HRESULT e2d::MediaAsyncCallback::GetParameters(DWORD *, DWORD *)
{
	// Implementation of this method is optional.
	return E_NOTIMPL;
}

HRESULT e2d::MediaAsyncCallback::Invoke(IMFAsyncResult *pResult)
{
	MediaEventType meType = MEUnknown;  // Event type

	IMFMediaEvent *pEvent = NULL;

	// Get the event from the event queue.
	HRESULT hr = m_pSession->EndGetEvent(pResult, &pEvent);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the event type. 
	hr = pEvent->GetType(&meType);
	if (FAILED(hr))
	{
		goto done;
	}

	if (meType == MESessionClosed)
	{
		// The session was closed. 
		// The application is waiting on the m_hCloseEvent event handle. 
		SetEvent(m_hCloseEvent);
	}
	else
	{
		// For all other events, get the next event in the queue.
		hr = m_pSession->BeginGetEvent(this, NULL);
		if (FAILED(hr))
		{
			goto done;
		}
	}

	// Check the application state. 

	// If a call to IMFMediaSession::Close is pending, it means the 
	// application is waiting on the m_hCloseEvent event and
	// the application's message loop is blocked. 

	// Otherwise, post a private window message to the application. 

	//if (m_state != Closing)
	{
		// Leave a reference count on the event.
		pEvent->AddRef();

		::PostMessage(m_hwnd, WM_APP + 1, (WPARAM)pEvent, (LPARAM)meType);
	}

done:
	SafeRelease(pEvent);
	return S_OK;
}

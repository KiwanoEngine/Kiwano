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

#include "..\e2dtool.h"
#include <cguid.h>

#define MF_CLASS_NAME		L"MediaFoundationCallbackWnd"
#define WM_APP_PLAYER_EVENT	(WM_APP + 1)


namespace
{
	HINSTANCE media_instance = nullptr;

	inline void Trace(wchar_t* msg)
	{
		WARN("Trace error: %s.", msg);
	}

	inline void Trace(wchar_t* msg, HRESULT hr)
	{
		WARN("Trace error: %s (%#X).", msg, hr);
	}
}


e2d::Music::Music()
	: m_pSession(nullptr)
	, m_pSource(nullptr)
	, m_hwndEvent(nullptr)
	, m_state(State::Closed)
	, m_hCloseEvent(nullptr)
	, m_nTimes(0)
{
	if (!media_instance)
	{
		media_instance = ::GetModuleHandle(NULL);

		WNDCLASS wc;
		wc.style = 0;
		wc.lpfnWndProc = Music::MediaProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = media_instance;
		wc.hIcon = 0;
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = MF_CLASS_NAME;

		if (!::RegisterClass(&wc))
		{
			return;
		}
	}

	m_hwndEvent = ::CreateWindowExW(
		WS_EX_APPWINDOW,
		MF_CLASS_NAME,
		NULL,
		WS_POPUPWINDOW,
		0, 0, 0, 0,
		NULL,
		NULL,
		media_instance,
		NULL
	);

	if (m_hwndEvent)
	{
		::SetWindowLongW(m_hwndEvent, GWLP_USERDATA, (LONG_PTR)this);
	}

	m_hCloseEvent = ::CreateEventW(NULL, FALSE, FALSE, NULL);
	if (m_hCloseEvent == NULL)
	{
		ThrowIfFailed(
			HRESULT_FROM_WIN32(GetLastError())
		);
	}
}

e2d::Music::Music(const e2d::String & file_path)
{
	this->Load(file_path);
}

e2d::Music::Music(const Resource& res)
{
	this->Load(res);
}

e2d::Music::~Music()
{
	Close();

	if (m_hCloseEvent)
	{
		::CloseHandle(m_hCloseEvent);
		m_hCloseEvent = NULL;
	}

	DestroyWindow(m_hwndEvent);
}

bool e2d::Music::Load(const e2d::String & file_path)
{
	// 1. Create a new media session.
	// 2. Create the media source.
	// 3. Create the topology.
	// 4. Queue the topology [asynchronous]
	// 5. Start playback [asynchronous - does not happen in this method.]

	IMFTopology *pTopology = NULL;
	IMFPresentationDescriptor* pSourcePD = NULL;

	// Close the old session, if any.
	Close();

	// Create the media session.
	HRESULT hr = MFCreateMediaSession(NULL, &m_pSession);
	if (FAILED(hr))
	{
		goto done;
	}

	// Start pulling events from the media session
	hr = m_pSession->BeginGetEvent((IMFAsyncCallback*)this, NULL);
	if (FAILED(hr))
	{
		goto done;
	}

	// Create the media source.
	hr = CreateMediaSource(static_cast<WCHAR*>(file_path), &m_pSource);
	if (FAILED(hr))
	{
		goto done;
	}

	// Create the presentation descriptor for the media source.
	hr = m_pSource->CreatePresentationDescriptor(&pSourcePD);
	if (FAILED(hr))
	{
		goto done;
	}

	// Create a partial topology.
	hr = CreatePlaybackTopology(m_pSource, pSourcePD, &pTopology);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the topology on the media session.
	hr = m_pSession->SetTopology(0, pTopology);
	if (FAILED(hr))
	{
		goto done;
	}

	m_state = State::Loaded;

	// If SetTopology succeeds, the media session will queue an 
	// MESessionTopologySet event.

done:
	if (FAILED(hr))
	{
		m_state = State::Closed;
	}

	SafeRelease(pSourcePD);
	SafeRelease(pTopology);
	return hr;
}

bool e2d::Music::Load(const Resource& res)
{
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	return false;
}

bool e2d::Music::Play(int loop_count)
{
	if (m_pSession == NULL || m_pSource == NULL)
	{
		return false;
	}
	HRESULT hr = StartPlayback();
	if (SUCCEEDED(hr))
	{
		m_nTimes = loop_count;
		return true;
	}
	return false;
}

void e2d::Music::Pause()
{
	if (m_state != State::Started)
	{
		return;
	}
	if (m_pSession == NULL || m_pSource == NULL)
	{
		return;
	}

	HRESULT hr = m_pSession->Pause();
	if (SUCCEEDED(hr))
	{
		m_state = State::Paused;
	}
}

void e2d::Music::Stop()
{
	if (m_state != State::Started && m_state != State::Paused)
	{
		return;
	}
	if (m_pSession == NULL)
	{
		return;
	}

	HRESULT hr = m_pSession->Stop();
	if (SUCCEEDED(hr))
	{
		m_state = State::Stopped;
	}
}

void e2d::Music::Close()
{
	//  The IMFMediaSession::Close method is asynchronous, but the 
	//  e2d::Music::CloseSession method waits on the MESessionClosed event.
	//  
	//  MESessionClosed is guaranteed to be the last event that the 
	//  media session fires.

	HRESULT hr = S_OK;

	// First close the media session.
	if (m_pSession)
	{
		DWORD dwWaitResult = 0;

		m_state = State::Closing;

		hr = m_pSession->Close();

		// Wait for the close operation to complete
		if (SUCCEEDED(hr))
		{
			WaitForSingleObject(m_hCloseEvent, 5000);
		}
	}

	// Complete shutdown operations.
	if (SUCCEEDED(hr))
	{
		// Shut down the media source. (Synchronous operation, no events.)
		if (m_pSource)
		{
			(void)m_pSource->Shutdown();
		}
		// Shut down the media session. (Synchronous operation, no events.)
		if (m_pSession)
		{
			(void)m_pSession->Shutdown();
		}
	}

	SafeRelease(m_pSource);
	SafeRelease(m_pSession);

	m_state = State::Closed;
}

bool e2d::Music::IsPlaying() const
{
	return m_state == State::Started;
}

bool e2d::Music::SetVolume(float volume)
{
	if (!m_pSession)
		return false;

	IMFSimpleAudioVolume* pAudioVolume = NULL;
	HRESULT hr = GetSimpleAudioVolume(&pAudioVolume);
	if (SUCCEEDED(hr))
	{
		volume = std::min(std::max(volume, 0.f), 1.f);
		hr = pAudioVolume->SetMasterVolume(volume);
	}
	
	SafeRelease(pAudioVolume);
	return SUCCEEDED(hr);
}

float e2d::Music::GetVolume() const
{
	if (!m_pSession)
		return 0.f;

	float volume = 0.f;
	IMFSimpleAudioVolume* pAudioVolume = NULL;
	HRESULT hr = GetSimpleAudioVolume(&pAudioVolume);
	if (SUCCEEDED(hr))
	{
		hr = pAudioVolume->GetMasterVolume(&volume);
	}
	SafeRelease(pAudioVolume);

	if (SUCCEEDED(hr))
	{
		return volume;
	}
	return 0.f;
}

HRESULT e2d::Music::QueryInterface(REFIID riid, void** ppv)
{
	static const QITAB qit[] =
	{
		QITABENT(Music, IMFAsyncCallback),
		{ 0 }
	};
	return QISearch(this, qit, riid, ppv);
}

ULONG e2d::Music::AddRef()
{
	return InterlockedIncrement(&m_nRefCount);
}

ULONG e2d::Music::Release()
{
	ULONG uCount = InterlockedDecrement(&m_nRefCount);
	if (uCount == 0)
	{
		delete this;
	}
	return uCount;
}

HRESULT e2d::Music::GetParameters(DWORD *, DWORD *)
{
	// Implementation of this method is optional.
	return E_NOTIMPL;
}

HRESULT e2d::Music::Invoke(IMFAsyncResult *pResult)
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

	if (m_state != State::Closing)
	{
		// Leave a reference count on the event.
		pEvent->AddRef();

		::PostMessage(m_hwndEvent, WM_APP_PLAYER_EVENT, (WPARAM)pEvent, (LPARAM)meType);
	}

done:
	SafeRelease(pEvent);
	return S_OK;
}

HRESULT e2d::Music::StartPlayback()
{
	PROPVARIANT varStart;
	PropVariantInit(&varStart);

	HRESULT hr = m_pSession->Start(&GUID_NULL, &varStart);
	if (SUCCEEDED(hr))
	{
		// Note: Start is an asynchronous operation. However, we
		// can treat our state as being already started. If Start
		// fails later, we'll get an MESessionStarted event with
		// an error code, and we will update our state then.
		m_state = State::Started;
	}
	PropVariantClear(&varStart);
	return hr;
}

HRESULT e2d::Music::HandleEvent(UINT_PTR pEventPtr)
{
	HRESULT hrStatus = S_OK;
	MediaEventType meType = MEUnknown;

	IMFMediaEvent *pEvent = (IMFMediaEvent*)pEventPtr;

	if (pEvent == NULL)
	{
		return E_POINTER;
	}

	// Get the event type.
	HRESULT hr = pEvent->GetType(&meType);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the event status. If the operation that triggered the event 
	// did not succeed, the status is a failure code.
	hr = pEvent->GetStatus(&hrStatus);

	// Check if the async operation succeeded.
	if (SUCCEEDED(hr) && FAILED(hrStatus))
	{
		hr = hrStatus;
	}

	if (FAILED(hr))
	{
		goto done;
	}

	switch (meType)
	{
	case MEEndOfPresentation:
		printf("%d\n", m_nTimes);
		if (m_nTimes)
		{
			--m_nTimes;
			hr = StartPlayback();
		}
		else
		{
			m_state = State::Stopped;
			hr = S_OK;
		}
		break;

	case MENewPresentation:
		hr = OnNewPresentation(pEvent);
		break;

	default:
		break;
	}

done:
	SafeRelease(pEvent);
	return hr;
}

HRESULT e2d::Music::OnNewPresentation(IMFMediaEvent *pEvent)
{
	IMFPresentationDescriptor *pPD = NULL;
	IMFTopology *pTopology = NULL;

	// Get the presentation descriptor from the event.
	PROPVARIANT var;
	HRESULT hr = pEvent->GetValue(&var);
	if (FAILED(hr))
	{
		goto done;
	}

	if (var.vt == VT_UNKNOWN)
	{
		hr = var.punkVal->QueryInterface(&pPD);
	}
	else
	{
		hr = MF_E_INVALIDTYPE;
	}
	PropVariantClear(&var);

	if (FAILED(hr))
	{
		goto done;
	}

	// Create a partial topology.
	hr = CreatePlaybackTopology(m_pSource, pPD, &pTopology);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the topology on the media session.
	hr = m_pSession->SetTopology(0, pTopology);
	if (FAILED(hr))
	{
		goto done;
	}

	m_state = State::Loaded;

done:
	SafeRelease(pTopology);
	SafeRelease(pPD);
	return S_OK;
}

HRESULT e2d::Music::GetSimpleAudioVolume(IMFSimpleAudioVolume ** ppAudioVolume) const
{
	if (ppAudioVolume == NULL)
		return E_POINTER;

	IMFGetService* pGetService = NULL;

	HRESULT hr = m_pSession->QueryInterface(IID_IMFGetService, (void **)&pGetService);
	if (SUCCEEDED(hr))
	{
		hr = pGetService->GetService(MR_CAPTURE_POLICY_VOLUME_SERVICE, IID_PPV_ARGS(ppAudioVolume));
	}
	SafeRelease(pGetService);
	return hr;
}

HRESULT e2d::Music::CreateMediaSource(PCWSTR sURL, IMFMediaSource **ppSource)
{
	MF_OBJECT_TYPE ObjectType = MF_OBJECT_INVALID;

	IMFSourceResolver* pSourceResolver = NULL;
	IUnknown* pSource = NULL;

	// Create the source resolver.
	HRESULT hr = MFCreateSourceResolver(&pSourceResolver);
	if (FAILED(hr))
	{
		goto done;
	}

	// Use the source resolver to create the media source.

	// Note: For simplicity this sample uses the synchronous method to create 
	// the media source. However, creating a media source can take a noticeable
	// amount of time, especially for a network source. For a more responsive 
	// UI, use the asynchronous BeginCreateObjectFromURL method.

	hr = pSourceResolver->CreateObjectFromURL(
		sURL,                       // URL of the source.
		MF_RESOLUTION_MEDIASOURCE,  // Create a source object.
		NULL,                       // Optional property store.
		&ObjectType,        // Receives the created object type. 
		&pSource            // Receives a pointer to the media source.
	);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the IMFMediaSource interface from the media source.
	hr = pSource->QueryInterface(IID_PPV_ARGS(ppSource));

done:
	SafeRelease(pSourceResolver);
	SafeRelease(pSource);
	return hr;
}

HRESULT e2d::Music::CreateMediaSinkActivate(
	IMFStreamDescriptor *pSourceSD,
	IMFActivate **ppActivate)
{
	IMFMediaTypeHandler *pHandler = NULL;
	IMFActivate *pActivate = NULL;

	// Get the media type handler for the stream.
	HRESULT hr = pSourceSD->GetMediaTypeHandler(&pHandler);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the major media type.
	GUID guidMajorType;
	hr = pHandler->GetMajorType(&guidMajorType);
	if (FAILED(hr))
	{
		goto done;
	}

	// Create an IMFActivate object for the renderer, based on the media type.
	if (MFMediaType_Audio == guidMajorType)
	{
		// Create the audio renderer.
		hr = MFCreateAudioRendererActivate(&pActivate);
	}
	else if (MFMediaType_Video == guidMajorType)
	{
		// Create the video renderer.
		hr = MFCreateVideoRendererActivate(m_hwndEvent, &pActivate);
	}
	else
	{
		// Unknown stream type. 
		hr = E_FAIL;
		// Optionally, you could deselect this stream instead of failing.
	}
	if (FAILED(hr))
	{
		goto done;
	}

	// Return IMFActivate pointer to caller.
	*ppActivate = pActivate;
	(*ppActivate)->AddRef();

done:
	SafeRelease(pHandler);
	SafeRelease(pActivate);
	return hr;
}

HRESULT e2d::Music::AddSourceNode(
	IMFTopology *pTopology,
	IMFMediaSource *pSource,
	IMFPresentationDescriptor *pPD,
	IMFStreamDescriptor *pSD,
	IMFTopologyNode **ppNode)
{
	IMFTopologyNode *pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the attributes.
	hr = pNode->SetUnknown(MF_TOPONODE_SOURCE, pSource);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(pNode);
	return hr;
}

HRESULT e2d::Music::AddOutputNode(
	IMFTopology *pTopology,
	IMFActivate *pActivate,
	DWORD dwId,
	IMFTopologyNode **ppNode)
{
	IMFTopologyNode *pNode = NULL;

	// Create the node.
	HRESULT hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the object pointer.
	hr = pNode->SetObject(pActivate);
	if (FAILED(hr))
	{
		goto done;
	}

	// Set the stream sink ID attribute.
	hr = pNode->SetUINT32(MF_TOPONODE_STREAMID, dwId);
	if (FAILED(hr))
	{
		goto done;
	}

	hr = pNode->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
	if (FAILED(hr))
	{
		goto done;
	}

	// Add the node to the topology.
	hr = pTopology->AddNode(pNode);
	if (FAILED(hr))
	{
		goto done;
	}

	// Return the pointer to the caller.
	*ppNode = pNode;
	(*ppNode)->AddRef();

done:
	SafeRelease(pNode);
	return hr;
}

HRESULT e2d::Music::AddBranchToPartialTopology(
	IMFTopology *pTopology,
	IMFMediaSource *pSource,
	IMFPresentationDescriptor *pPD,
	DWORD iStream)
{
	//  Add a topology branch for one stream.
	//
	//  For each stream, this function does the following:
	//
	//    1. Creates a source node associated with the stream. 
	//    2. Creates an output node for the renderer. 
	//    3. Connects the two nodes.
	//
	//  The media session will add any decoders that are needed.

	IMFStreamDescriptor *pSD = NULL;
	IMFActivate         *pSinkActivate = NULL;
	IMFTopologyNode     *pSourceNode = NULL;
	IMFTopologyNode     *pOutputNode = NULL;

	BOOL fSelected = FALSE;

	HRESULT hr = pPD->GetStreamDescriptorByIndex(iStream, &fSelected, &pSD);
	if (FAILED(hr))
	{
		goto done;
	}

	if (fSelected)
	{
		// Create the media sink activation object.
		hr = CreateMediaSinkActivate(pSD, &pSinkActivate);
		if (FAILED(hr))
		{
			goto done;
		}

		// Add a source node for this stream.
		hr = AddSourceNode(pTopology, pSource, pPD, pSD, &pSourceNode);
		if (FAILED(hr))
		{
			goto done;
		}

		// Create the output node for the renderer.
		hr = AddOutputNode(pTopology, pSinkActivate, 0, &pOutputNode);
		if (FAILED(hr))
		{
			goto done;
		}

		// Connect the source node to the output node.
		hr = pSourceNode->ConnectOutput(0, pOutputNode, 0);
	}
	// else: If not selected, don't add the branch. 

done:
	SafeRelease(pSD);
	SafeRelease(pSinkActivate);
	SafeRelease(pSourceNode);
	SafeRelease(pOutputNode);
	return hr;
}

HRESULT e2d::Music::CreatePlaybackTopology(
	IMFMediaSource *pSource,
	IMFPresentationDescriptor *pPD,
	IMFTopology **ppTopology)
{
	IMFTopology *pTopology = NULL;
	DWORD cSourceStreams = 0;

	// Create a new topology.
	HRESULT hr = MFCreateTopology(&pTopology);
	if (FAILED(hr))
	{
		goto done;
	}

	// Get the number of streams in the media source.
	hr = pPD->GetStreamDescriptorCount(&cSourceStreams);
	if (FAILED(hr))
	{
		goto done;
	}

	// For each stream, create the topology nodes and add them to the topology.
	for (DWORD i = 0; i < cSourceStreams; i++)
	{
		hr = AddBranchToPartialTopology(pTopology, pSource, pPD, i);
		if (FAILED(hr))
		{
			goto done;
		}
	}

	// Return the IMFTopology pointer to the caller.
	*ppTopology = pTopology;
	(*ppTopology)->AddRef();

done:
	SafeRelease(pTopology);
	return hr;
}

LRESULT e2d::Music::MediaProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
	case WM_APP_PLAYER_EVENT:
		{
			Music * music = reinterpret_cast<Music*>(
				static_cast<LONG_PTR>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA))
			);
			if (music)
			{
				music->HandleEvent(wParam);
			}
		}
		break;

	default:
		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
	}
	return 0;
}

// Copyright (c) 2016-2018 Kiwano - Nomango
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
// _Out_ OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "FontCollectionLoader.h"

namespace kiwano
{
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// FontCollectionLoader
	//
	////////////////////////////////////////////////////////////////////////////////////////

	class FontCollectionLoader
		: public IFontCollectionLoader
	{
	public:
		FontCollectionLoader()
			: refCount_(0)
		{
		}

		// IUnknown methods
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

		// IDWriteFontCollectionLoader methods
		virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
			IDWriteFactory* pFactory,
			void const* collectionKey,
			UINT32 collectionKeySize,
			_Out_ IDWriteFontFileEnumerator** fontFileEnumerator
		);

	private:
		ULONG refCount_;
	};

	HRESULT IFontCollectionLoader::Create(_Out_ IFontCollectionLoader** ppCollectionLoader)
	{
		HRESULT hr = S_OK;

		if (!ppCollectionLoader)
		{
			hr = E_POINTER;
		}

		if (SUCCEEDED(hr))
		{
			FontCollectionLoader* pCollectionLoader = new (std::nothrow) FontCollectionLoader;
			hr = pCollectionLoader ? S_OK : E_OUTOFMEMORY;

			if (SUCCEEDED(hr))
			{
				DX::SafeRelease(*ppCollectionLoader);
				(*ppCollectionLoader) = DX::SafeAcquire(pCollectionLoader);
			}
		}
		return hr;
	}

	HRESULT STDMETHODCALLTYPE FontCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE FontCollectionLoader::AddRef()
	{
		return InterlockedIncrement(&refCount_);
	}

	ULONG STDMETHODCALLTYPE FontCollectionLoader::Release()
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	HRESULT STDMETHODCALLTYPE FontCollectionLoader::CreateEnumeratorFromKey(
		IDWriteFactory* pFactory,
		void const* collectionKey,
		UINT32 collectionKeySize,
		_Out_ IDWriteFontFileEnumerator** fontFileEnumerator
	)
	{
		HRESULT hr = S_OK;

		if (collectionKey == NULL || collectionKeySize != sizeof(String))
			hr = E_INVALIDARG;

		if (SUCCEEDED(hr))
		{
			IFontFileEnumerator* pEnumerator = NULL;
			hr = IFontFileEnumerator::Create(&pEnumerator, pFactory);

			if (SUCCEEDED(hr))
			{
				String const* filePath = static_cast<String const*>(collectionKey);
				UINT32 const fileCount = collectionKeySize / sizeof(String);

				hr = pEnumerator->SetFilePaths(filePath, fileCount);
			}

			if (SUCCEEDED(hr))
			{
				*fontFileEnumerator = DX::SafeAcquire(pEnumerator);
			}
		}
		return hr;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	//
	// FontFileEnumerator
	//
	////////////////////////////////////////////////////////////////////////////////////////

	class FontFileEnumerator
		: public IFontFileEnumerator
	{
	public:
		FontFileEnumerator();

		STDMETHOD(Initialize)(
			IDWriteFactory* pFactory
		);

		STDMETHOD(SetFilePaths)(
			String const* filePath,
			UINT32 const fileCount
		);

		~FontFileEnumerator()
		{
			DX::SafeRelease(currentFile_);
			DX::SafeRelease(pFactory_);
		}

		// IUnknown methods
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, _Out_ void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

		// IDWriteFontFileEnumerator methods
		virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ BOOL* hasCurrentFile);
		virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile);

	private:
		ULONG refCount_;

		IDWriteFactory* pFactory_;
		IDWriteFontFile* currentFile_;
		Vector<String> filePaths_;
		size_t nextIndex_;
	};

	HRESULT IFontFileEnumerator::Create(_Out_ IFontFileEnumerator** ppEnumerator, IDWriteFactory* pFactory)
	{
		HRESULT hr = S_OK;

		if (!ppEnumerator)
		{
			hr = E_POINTER;
		}

		if (SUCCEEDED(hr))
		{
			FontFileEnumerator* pEnumerator = new (std::nothrow) FontFileEnumerator;
			hr = pEnumerator ? S_OK : E_OUTOFMEMORY;

			if (SUCCEEDED(hr))
			{
				hr = pEnumerator->Initialize(pFactory);
			}

			if (SUCCEEDED(hr))
			{
				DX::SafeRelease(*ppEnumerator);
				(*ppEnumerator) = DX::SafeAcquire(pEnumerator);
			}
		}
		return hr;
	}

	FontFileEnumerator::FontFileEnumerator()
		: refCount_(0)
		, pFactory_(NULL)
		, currentFile_(NULL)
		, nextIndex_(0)
	{
	}

	STDMETHODIMP FontFileEnumerator::Initialize(
		IDWriteFactory* pFactory
	)
	{
		if (pFactory)
		{
			pFactory_ = DX::SafeAcquire(pFactory);
			return S_OK;
		}
		return E_INVALIDARG;
	}

	STDMETHODIMP FontFileEnumerator::SetFilePaths(
		String const* filePath,
		UINT32 const fileCount
	)
	{
		try
		{
			filePaths_.assign(filePath, filePath + fileCount);
		}
		catch (std::bad_alloc&)
		{
			return E_OUTOFMEMORY;
		}
		catch (...)
		{
			return E_FAIL;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE FontFileEnumerator::QueryInterface(REFIID iid, _Out_ void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE FontFileEnumerator::AddRef()
	{
		return InterlockedIncrement(&refCount_);
	}

	ULONG STDMETHODCALLTYPE FontFileEnumerator::Release()
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	HRESULT STDMETHODCALLTYPE FontFileEnumerator::MoveNext(_Out_ BOOL* hasCurrentFile)
	{
		HRESULT hr = S_OK;

		*hasCurrentFile = FALSE;
		DX::SafeRelease(currentFile_);

		if (nextIndex_ < filePaths_.size())
		{
			hr = pFactory_->CreateFontFileReference(
				filePaths_[nextIndex_].c_str(),
				NULL,
				&currentFile_
			);

			if (SUCCEEDED(hr))
			{
				*hasCurrentFile = TRUE;

				++nextIndex_;
			}
		}

		return hr;
	}

	HRESULT STDMETHODCALLTYPE FontFileEnumerator::GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile)
	{
		*fontFile = DX::SafeAcquire(currentFile_);

		return (currentFile_ != NULL) ? S_OK : E_FAIL;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	//
	// ResourceFontCollectionLoader
	//
	////////////////////////////////////////////////////////////////////////////////////////

	class ResourceFontCollectionLoader
		: public IResourceFontCollectionLoader
	{
	public:
		ResourceFontCollectionLoader(IDWriteFontFileLoader* pFileLoader)
			: refCount_(0)
			, pFileLoader_(pFileLoader)
		{
		}

		// IUnknown methods
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

		// IDWriteFontCollectionLoader methods
		virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
			IDWriteFactory* pFactory,
			void const* collectionKey,
			UINT32 collectionKeySize,
			_Out_ IDWriteFontFileEnumerator** fontFileEnumerator
		);

	private:
		ULONG refCount_;
		IDWriteFontFileLoader* pFileLoader_;
	};

	HRESULT IResourceFontCollectionLoader::Create(_Out_ IResourceFontCollectionLoader** ppCollectionLoader, IDWriteFontFileLoader* pFileLoader)
	{
		HRESULT hr = S_OK;

		if (!ppCollectionLoader)
		{
			hr = E_POINTER;
		}

		if (SUCCEEDED(hr))
		{
			ResourceFontCollectionLoader* pCollectionLoader = new (std::nothrow) ResourceFontCollectionLoader(pFileLoader);
			hr = pCollectionLoader ? S_OK : E_OUTOFMEMORY;

			if (SUCCEEDED(hr))
			{
				DX::SafeRelease(*ppCollectionLoader);
				(*ppCollectionLoader) = DX::SafeAcquire(pCollectionLoader);
			}
		}
		return hr;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE ResourceFontCollectionLoader::AddRef()
	{
		return InterlockedIncrement(&refCount_);
	}

	ULONG STDMETHODCALLTYPE ResourceFontCollectionLoader::Release()
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontCollectionLoader::CreateEnumeratorFromKey(
		IDWriteFactory* pFactory,
		void const* collectionKey,
		UINT32 collectionKeySize,
		_Out_ IDWriteFontFileEnumerator** fontFileEnumerator
	)
	{
		HRESULT hr = S_OK;

		if (collectionKey == NULL || collectionKeySize % sizeof(UINT) != 0)
			hr = E_INVALIDARG;

		if (SUCCEEDED(hr))
		{
			IResourceFontFileEnumerator* pEnumerator = NULL;
			hr = IResourceFontFileEnumerator::Create(&pEnumerator, pFactory, pFileLoader_);

			if (SUCCEEDED(hr))
			{
				UINT const* resourceID = static_cast<const UINT*>(collectionKey);
				UINT32 const resourceCount = collectionKeySize / sizeof(UINT);

				hr = pEnumerator->SetResources(resourceID, resourceCount);
			}

			if (SUCCEEDED(hr))
			{
				*fontFileEnumerator = DX::SafeAcquire(pEnumerator);
			}
		}
		return hr;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	//
	// ResourceFontFileLoader
	//
	////////////////////////////////////////////////////////////////////////////////////////

	class ResourceFontFileLoader
		: public IResourceFontFileLoader
	{
	public:
		ResourceFontFileLoader()
			: refCount_(0)
		{
		}

		// IUnknown methods
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

		// IDWriteFontFileLoader methods
		virtual HRESULT STDMETHODCALLTYPE CreateStreamFromKey(
			void const* fontFileReferenceKey,
			UINT32 fontFileReferenceKeySize,
			_Out_ IDWriteFontFileStream** fontFileStream
		);

	private:
		ULONG refCount_;
	};

	HRESULT IResourceFontFileLoader::Create(_Out_ IResourceFontFileLoader** ppFileLoader)
	{
		HRESULT hr = S_OK;

		if (!ppFileLoader)
		{
			hr = E_POINTER;
		}

		if (SUCCEEDED(hr))
		{
			ResourceFontFileLoader* pFileLoader = new (std::nothrow) ResourceFontFileLoader;
			hr = pFileLoader ? S_OK : E_OUTOFMEMORY;

			if (SUCCEEDED(hr))
			{
				(*ppFileLoader) = DX::SafeAcquire(pFileLoader);
			}
		}
		return hr;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileLoader::QueryInterface(REFIID iid, void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileLoader))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE ResourceFontFileLoader::AddRef()
	{
		return InterlockedIncrement(&refCount_);
	}

	ULONG STDMETHODCALLTYPE ResourceFontFileLoader::Release()
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileLoader::CreateStreamFromKey(
		void const* fontFileReferenceKey,
		UINT32 fontFileReferenceKeySize,
		_Out_ IDWriteFontFileStream** fontFileStream
	)
	{
		HRESULT hr = S_OK;

		// Make sure the key is the right size.
		if (fontFileReferenceKeySize != sizeof(UINT))
			hr = E_INVALIDARG;

		if (SUCCEEDED(hr))
		{
			// Create the pFileStream object.
			IResourceFontFileStream* pFileStream = NULL;
			UINT resourceID = *static_cast<UINT const*>(fontFileReferenceKey);

			hr = IResourceFontFileStream::Create(&pFileStream, resourceID);

			if (SUCCEEDED(hr))
			{
				DX::SafeRelease(*fontFileStream);
				*fontFileStream = DX::SafeAcquire(pFileStream);
			}
		}
		return hr;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	//
	// ResourceFontFileEnumerator
	//
	////////////////////////////////////////////////////////////////////////////////////////

	class ResourceFontFileEnumerator
		: public IResourceFontFileEnumerator
	{
	public:
		ResourceFontFileEnumerator();

		STDMETHOD(Initialize)(
			IDWriteFactory* pFactory,
			IDWriteFontFileLoader* pLoader
		);

		STDMETHOD(SetResources)(
			UINT const* resourceID,
			UINT32 const resourceCount
		);

		~ResourceFontFileEnumerator()
		{
			DX::SafeRelease(currentFile_);
			DX::SafeRelease(pFactory_);
		}

		// IUnknown methods
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, _Out_ void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

		// IDWriteFontFileEnumerator methods
		virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ BOOL* hasCurrentFile);
		virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile);

	private:
		ULONG refCount_;

		IDWriteFactory* pFactory_;
		IDWriteFontFile* currentFile_;
		IDWriteFontFileLoader* pLoader_;
		Vector<UINT> resourceIDs_;
		size_t nextIndex_;
	};

	HRESULT IResourceFontFileEnumerator::Create(_Out_ IResourceFontFileEnumerator** ppEnumerator, IDWriteFactory* pFactory, IDWriteFontFileLoader* pFileLoader)
	{
		HRESULT hr = S_OK;

		if (!ppEnumerator)
		{
			hr = E_POINTER;
		}

		if (SUCCEEDED(hr))
		{
			ResourceFontFileEnumerator* pEnumerator = new (std::nothrow) ResourceFontFileEnumerator;
			hr = pEnumerator ? S_OK : E_OUTOFMEMORY;

			if (SUCCEEDED(hr))
			{
				hr = pEnumerator->Initialize(pFactory, pFileLoader);
			}

			if (SUCCEEDED(hr))
			{
				DX::SafeRelease(*ppEnumerator);
				(*ppEnumerator) = DX::SafeAcquire(pEnumerator);
			}
		}
		return hr;
	}

	ResourceFontFileEnumerator::ResourceFontFileEnumerator()
		: refCount_(0)
		, pFactory_(NULL)
		, currentFile_(NULL)
		, nextIndex_(0)
		, pLoader_(NULL)
	{
	}

	STDMETHODIMP ResourceFontFileEnumerator::Initialize(
		IDWriteFactory* pFactory,
		IDWriteFontFileLoader* pLoader
	)
	{
		if (pFactory && pLoader)
		{
			pFactory_ = DX::SafeAcquire(pFactory);
			pLoader_ = DX::SafeAcquire(pLoader);
			return S_OK;
		}
		return E_INVALIDARG;
	}

	STDMETHODIMP ResourceFontFileEnumerator::SetResources(
		UINT const* resourceID,
		UINT32 const resourceCount
	)
	{
		try
		{
			resourceIDs_.assign(resourceID, resourceID + resourceCount);
		}
		catch (std::bad_alloc&)
		{
			return E_OUTOFMEMORY;
		}
		catch (...)
		{
			return E_FAIL;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileEnumerator::QueryInterface(REFIID iid, _Out_ void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE ResourceFontFileEnumerator::AddRef()
	{
		return InterlockedIncrement(&refCount_);
	}

	ULONG STDMETHODCALLTYPE ResourceFontFileEnumerator::Release()
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileEnumerator::MoveNext(_Out_ BOOL* hasCurrentFile)
	{
		HRESULT hr = S_OK;

		*hasCurrentFile = FALSE;
		DX::SafeRelease(currentFile_);

		if (nextIndex_ < resourceIDs_.size())
		{
			hr = pFactory_->CreateCustomFontFileReference(
				&resourceIDs_[nextIndex_],
				sizeof(UINT),
				pLoader_,
				&currentFile_
			);

			if (SUCCEEDED(hr))
			{
				*hasCurrentFile = TRUE;

				++nextIndex_;
			}
		}

		return hr;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileEnumerator::GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile)
	{
		*fontFile = DX::SafeAcquire(currentFile_);

		return (currentFile_ != NULL) ? S_OK : E_FAIL;
	}

	
	////////////////////////////////////////////////////////////////////////////////////////
	//
	// ResourceFontFileStream
	//
	////////////////////////////////////////////////////////////////////////////////////////

	class ResourceFontFileStream
		: public IResourceFontFileStream
	{
	public:
		ResourceFontFileStream();

		STDMETHOD(Initialize)(
			const UINT resourceID
		);

		// IUnknown methods
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

		// IDWriteFontFileStream methods
		virtual HRESULT STDMETHODCALLTYPE ReadFileFragment(
			void const** fragmentStart,
			UINT64 fileOffset,
			UINT64 fragmentSize,
			_Out_ void** fragmentContext
		);

		virtual void STDMETHODCALLTYPE ReleaseFileFragment(
			void* fragmentContext
		);

		virtual HRESULT STDMETHODCALLTYPE GetFileSize(
			_Out_ UINT64* fileSize
		);

		virtual HRESULT STDMETHODCALLTYPE GetLastWriteTime(
			_Out_ UINT64* lastWriteTime
		);

		bool IsInitialized()
		{
			return resourcePtr_ != NULL;
		}

	private:
		ULONG refCount_;
		LPVOID resourcePtr_;
		DWORD resourceSize_;
	};

	HRESULT IResourceFontFileStream::Create(_Out_ IResourceFontFileStream** ppStream, const UINT resourceID)
	{
		HRESULT hr = S_OK;

		if (!ppStream)
		{
			hr = E_POINTER;
		}

		if (SUCCEEDED(hr))
		{
			ResourceFontFileStream* pFileStream = new (std::nothrow) ResourceFontFileStream;
			hr = pFileStream ? S_OK : E_OUTOFMEMORY;

			if (SUCCEEDED(hr))
			{
				hr = pFileStream->Initialize(resourceID);
			}

			if (SUCCEEDED(hr))
			{
				DX::SafeRelease(*ppStream);
				(*ppStream) = DX::SafeAcquire(pFileStream);
			}
		}
		return hr;
	}

	ResourceFontFileStream::ResourceFontFileStream()
		: refCount_(0)
		, resourcePtr_(NULL)
		, resourceSize_(0)
	{
	}

	STDMETHODIMP ResourceFontFileStream::Initialize(
		const UINT resourceID
	)
	{
		HRESULT hr = S_OK;

		if (!Resource{ resourceID, RT_FONT }.Load(resourcePtr_, resourceSize_))
		{
			hr = E_FAIL;
		}
		return hr;
	}

	// IUnknown methods
	HRESULT STDMETHODCALLTYPE ResourceFontFileStream::QueryInterface(REFIID iid, void** ppvObject)
	{
		if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileStream))
		{
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		else
		{
			*ppvObject = NULL;
			return E_NOINTERFACE;
		}
	}

	ULONG STDMETHODCALLTYPE ResourceFontFileStream::AddRef()
	{
		return InterlockedIncrement(&refCount_);
	}

	ULONG STDMETHODCALLTYPE ResourceFontFileStream::Release()
	{
		ULONG newCount = InterlockedDecrement(&refCount_);
		if (newCount == 0)
			delete this;

		return newCount;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileStream::ReadFileFragment(
		void const** fragmentStart,
		UINT64 fileOffset,
		UINT64 fragmentSize,
		_Out_ void** fragmentContext
	)
	{
		// The pLoader is responsible for doing a bounds check.
		if (fileOffset <= resourceSize_ &&
			fragmentSize <= resourceSize_ - fileOffset)
		{
			*fragmentStart = static_cast<BYTE const*>(resourcePtr_) + static_cast<size_t>(fileOffset);
			*fragmentContext = NULL;
			return S_OK;
		}
		else
		{
			*fragmentStart = NULL;
			*fragmentContext = NULL;
			return E_FAIL;
		}
	}

	void STDMETHODCALLTYPE ResourceFontFileStream::ReleaseFileFragment(
		void* fragmentContext
	)
	{
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileStream::GetFileSize(
		_Out_ UINT64* fileSize
	)
	{
		*fileSize = resourceSize_;
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE ResourceFontFileStream::GetLastWriteTime(
		_Out_ UINT64* lastWriteTime
	)
	{
		*lastWriteTime = 0;
		return E_NOTIMPL;
	}

}

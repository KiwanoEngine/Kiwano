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

#include <kiwano/render/DirectX/FontLoader.h>

namespace kiwano
{
namespace graphics
{
namespace directx
{
////////////////////////////////////////////////////////////////////////////////////////
//
// FontFileCollectionLoader
//
////////////////////////////////////////////////////////////////////////////////////////

class FontFileCollectionLoader : public IFontFileCollectionLoader
{
public:
    FontFileCollectionLoader()
        : refCount_(0)
    {
    }

    STDMETHOD(AddFilePaths)
    (const Vector<String>& filePaths, _Out_ LPVOID* pCollectionKey, _Out_ uint32_t* pCollectionKeySize);

    // IDWriteFontCollectionLoader methods
    virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(IDWriteFactory* pFactory, void const* collectionKey,
                                                              uint32_t                          collectionKeySize,
                                                              _Out_ IDWriteFontFileEnumerator** fontFileEnumerator);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE   AddRef();
    virtual ULONG STDMETHODCALLTYPE   Release();

private:
    ULONG refCount_;

    typedef Vector<String> FileCollection;
    Vector<FileCollection> filePaths_;
    Vector<size_t>         collectionKeys_;
};

HRESULT IFontFileCollectionLoader::Create(_Out_ IFontFileCollectionLoader** ppCollectionLoader)
{
    HRESULT hr = S_OK;

    if (!ppCollectionLoader)
    {
        hr = E_POINTER;
    }

    if (SUCCEEDED(hr))
    {
        FontFileCollectionLoader* pCollectionLoader = new (std::nothrow) FontFileCollectionLoader;
        hr                                          = pCollectionLoader ? S_OK : E_OUTOFMEMORY;

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*ppCollectionLoader);
            (*ppCollectionLoader) = DX::SafeAcquire(pCollectionLoader);
        }
    }
    return hr;
}

STDMETHODIMP FontFileCollectionLoader::AddFilePaths(const Vector<String>& filePaths, _Out_ LPVOID* pCollectionKey,
                                                    _Out_ uint32_t* pCollectionKeySize)
{
    if (!pCollectionKey || !pCollectionKeySize)
    {
        return E_INVALIDARG;
    }

    try
    {
        size_t collectionKey = filePaths_.size();
        collectionKeys_.push_back(collectionKey);
        filePaths_.push_back(filePaths);

        *pCollectionKey     = reinterpret_cast<LPVOID>(&collectionKeys_.back());
        *pCollectionKeySize = sizeof(collectionKey);
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

HRESULT STDMETHODCALLTYPE FontFileCollectionLoader::CreateEnumeratorFromKey(
    IDWriteFactory* pFactory, void const* collectionKey, uint32_t collectionKeySize,
    _Out_ IDWriteFontFileEnumerator** fontFileEnumerator)
{
    HRESULT hr = S_OK;

    if (collectionKey == NULL || collectionKeySize % sizeof(uint32_t) != 0)
        hr = E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
        IFontFileEnumerator* pEnumerator = NULL;

        hr = IFontFileEnumerator::Create(&pEnumerator, pFactory);
        if (SUCCEEDED(hr))
        {
            const uint32_t fileIndex = *static_cast<uint32_t const*>(collectionKey);

            hr = pEnumerator->SetFilePaths(filePaths_[fileIndex]);
        }

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*fontFileEnumerator);
            (*fontFileEnumerator) = pEnumerator;
        }
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE FontFileCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
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

ULONG STDMETHODCALLTYPE FontFileCollectionLoader::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG STDMETHODCALLTYPE FontFileCollectionLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount_);
    if (newCount == 0)
        delete this;

    return newCount;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// FontFileEnumerator
//
////////////////////////////////////////////////////////////////////////////////////////

class FontFileEnumerator : public IFontFileEnumerator
{
public:
    FontFileEnumerator();

    ~FontFileEnumerator()
    {
        DX::SafeRelease(currentFile_);
        DX::SafeRelease(pFactory_);
    }

    STDMETHOD(Initialize)(IDWriteFactory* pFactory);

    STDMETHOD(SetFilePaths)(const Vector<String>& filePaths);

    // IDWriteFontFileEnumerator methods
    virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ BOOL* hasCurrentFile);
    virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, _Out_ void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE   AddRef();
    virtual ULONG STDMETHODCALLTYPE   Release();

private:
    ULONG refCount_;

    IDWriteFactory*  pFactory_;
    IDWriteFontFile* currentFile_;
    Vector<String>   filePaths_;
    uint32_t         nextIndex_;
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
        hr                              = pEnumerator ? S_OK : E_OUTOFMEMORY;

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

STDMETHODIMP FontFileEnumerator::Initialize(IDWriteFactory* pFactory)
{
    if (pFactory)
    {
        pFactory_ = DX::SafeAcquire(pFactory);
        return S_OK;
    }
    return E_INVALIDARG;
}

STDMETHODIMP FontFileEnumerator::SetFilePaths(const Vector<String>& filePaths)
{
    try
    {
        filePaths_.assign(filePaths.begin(), filePaths.end());
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

HRESULT STDMETHODCALLTYPE FontFileEnumerator::MoveNext(_Out_ BOOL* hasCurrentFile)
{
    HRESULT hr = S_OK;

    *hasCurrentFile = FALSE;
    DX::SafeRelease(currentFile_);

    if (nextIndex_ < filePaths_.size())
    {
        WideString file_name = strings::NarrowToWide(filePaths_[nextIndex_]);

        hr = pFactory_->CreateFontFileReference(file_name.c_str(), NULL, &currentFile_);

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

////////////////////////////////////////////////////////////////////////////////////////
//
// FontResourceCollectionLoader
//
////////////////////////////////////////////////////////////////////////////////////////

class FontResourceCollectionLoader : public IFontResourceCollectionLoader
{
public:
    FontResourceCollectionLoader(IFontResourceLoader* pResLoader)
        : refCount_(0)
        , pResLoader(pResLoader)
    {
    }

    STDMETHOD(AddResources)
    (const Vector<BinaryData>& data, _Out_ LPVOID* pCollectionKey, _Out_ uint32_t* pCollectionKeySize);

    // IDWriteFontCollectionLoader methods
    virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(IDWriteFactory* pFactory, void const* collectionKey,
                                                              uint32_t                          collectionKeySize,
                                                              _Out_ IDWriteFontFileEnumerator** fontFileEnumerator);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE   AddRef();
    virtual ULONG STDMETHODCALLTYPE   Release();

private:
    ULONG                refCount_;
    IFontResourceLoader* pResLoader;

    typedef Vector<BinaryData> ResourceCollection;
    Vector<ResourceCollection> resources_;
    Vector<size_t>             collectionKeys_;
};

HRESULT IFontResourceCollectionLoader::Create(_Out_ IFontResourceCollectionLoader** ppCollectionLoader,
                                              IFontResourceLoader*                  pResLoader)
{
    HRESULT hr = S_OK;

    if (!ppCollectionLoader)
    {
        hr = E_POINTER;
    }

    if (SUCCEEDED(hr))
    {
        FontResourceCollectionLoader* pCollectionLoader = new (std::nothrow) FontResourceCollectionLoader(pResLoader);
        hr                                              = pCollectionLoader ? S_OK : E_OUTOFMEMORY;

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*ppCollectionLoader);
            (*ppCollectionLoader) = DX::SafeAcquire(pCollectionLoader);
        }
    }
    return hr;
}

STDMETHODIMP FontResourceCollectionLoader::AddResources(const Vector<BinaryData>& data, _Out_ LPVOID* pCollectionKey,
                                                        _Out_ uint32_t* pCollectionKeySize)
{
    if (!pCollectionKey || !pCollectionKeySize)
    {
        return E_INVALIDARG;
    }

    try
    {
        size_t collectionKey = resources_.size();
        collectionKeys_.push_back(collectionKey);
        resources_.push_back(data);

        *pCollectionKey     = reinterpret_cast<LPVOID>(&collectionKeys_.back());
        *pCollectionKeySize = sizeof(collectionKey);
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

HRESULT STDMETHODCALLTYPE FontResourceCollectionLoader::CreateEnumeratorFromKey(
    IDWriteFactory* pFactory, void const* collectionKey, uint32_t collectionKeySize,
    _Out_ IDWriteFontFileEnumerator** fontFileEnumerator)
{
    HRESULT hr = S_OK;

    if (collectionKey == NULL || collectionKeySize % sizeof(BinaryData*) != 0)
        hr = E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
        IFontResourceEnumerator* pEnumerator = NULL;

        hr = IFontResourceEnumerator::Create(&pEnumerator, pFactory, pResLoader);
        if (SUCCEEDED(hr))
        {
            const uint32_t resourceIndex = *static_cast<const uint32_t*>(collectionKey);

            hr = pEnumerator->SetResources(resources_[resourceIndex]);
        }

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*fontFileEnumerator);
            (*fontFileEnumerator) = pEnumerator;
        }
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE FontResourceCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
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

ULONG STDMETHODCALLTYPE FontResourceCollectionLoader::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG STDMETHODCALLTYPE FontResourceCollectionLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount_);
    if (newCount == 0)
        delete this;

    return newCount;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// FontResourceLoader
//
////////////////////////////////////////////////////////////////////////////////////////

class FontResourceLoader : public IFontResourceLoader
{
public:
    FontResourceLoader()
        : refCount_(0)
    {
    }

    // IDWriteFontFileLoader methods
    virtual HRESULT STDMETHODCALLTYPE CreateStreamFromKey(void const*                   fontFileReferenceKey,
                                                          uint32_t                      fontFileReferenceKeySize,
                                                          _Out_ IDWriteFontFileStream** fontFileStream);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE   AddRef();
    virtual ULONG STDMETHODCALLTYPE   Release();

private:
    ULONG refCount_;
};

HRESULT IFontResourceLoader::Create(_Out_ IFontResourceLoader** ppResLoader)
{
    HRESULT hr = S_OK;

    if (!ppResLoader)
    {
        hr = E_POINTER;
    }

    if (SUCCEEDED(hr))
    {
        FontResourceLoader* pResLoader = new (std::nothrow) FontResourceLoader;
        hr                             = pResLoader ? S_OK : E_OUTOFMEMORY;

        if (SUCCEEDED(hr))
        {
            (*ppResLoader) = DX::SafeAcquire(pResLoader);
        }
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE FontResourceLoader::CreateStreamFromKey(void const* fontFileReferenceKey,
                                                                  uint32_t    fontFileReferenceKeySize,
                                                                  _Out_ IDWriteFontFileStream** fontFileStream)
{
    HRESULT hr = S_OK;

    // Make sure the key is the right size.
    if (fontFileReferenceKeySize != sizeof(BinaryData))
        hr = E_INVALIDARG;

    if (SUCCEEDED(hr))
    {
        // Create the pFileStream object.
        IFontResourceStream* pFileStream = NULL;
        BinaryData           resource    = *static_cast<BinaryData const*>(fontFileReferenceKey);

        hr = IFontResourceStream::Create(&pFileStream, resource);

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*fontFileStream);
            (*fontFileStream) = pFileStream;
        }
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE FontResourceLoader::QueryInterface(REFIID iid, void** ppvObject)
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

ULONG STDMETHODCALLTYPE FontResourceLoader::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG STDMETHODCALLTYPE FontResourceLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount_);
    if (newCount == 0)
        delete this;

    return newCount;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// ResourceFontFileEnumerator
//
////////////////////////////////////////////////////////////////////////////////////////

class FontResourceEnumerator : public IFontResourceEnumerator
{
public:
    FontResourceEnumerator();

    ~FontResourceEnumerator()
    {
        DX::SafeRelease(currentFile_);
        DX::SafeRelease(pFactory_);
    }

    STDMETHOD(Initialize)(IDWriteFactory* pFactory, IFontResourceLoader* pResLoader);

    STDMETHOD(SetResources)(const Vector<BinaryData>& data);

    // IDWriteFontFileEnumerator methods
    virtual HRESULT STDMETHODCALLTYPE MoveNext(_Out_ BOOL* hasCurrentFile);
    virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, _Out_ void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE   AddRef();
    virtual ULONG STDMETHODCALLTYPE   Release();

private:
    ULONG refCount_;

    IDWriteFactory*      pFactory_;
    IDWriteFontFile*     currentFile_;
    IFontResourceLoader* pResLoader_;
    Vector<BinaryData>   resources_;
    uint32_t             nextIndex_;
};

HRESULT IFontResourceEnumerator::Create(_Out_ IFontResourceEnumerator** ppEnumerator, IDWriteFactory* pFactory,
                                        IFontResourceLoader* pResLoader)
{
    HRESULT hr = S_OK;

    if (!ppEnumerator)
    {
        hr = E_POINTER;
    }

    if (SUCCEEDED(hr))
    {
        FontResourceEnumerator* pEnumerator = new (std::nothrow) FontResourceEnumerator;

        hr = pEnumerator ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            hr = pEnumerator->Initialize(pFactory, pResLoader);
        }

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*ppEnumerator);
            (*ppEnumerator) = DX::SafeAcquire(pEnumerator);
        }
    }
    return hr;
}

FontResourceEnumerator::FontResourceEnumerator()
    : refCount_(0)
    , pFactory_(NULL)
    , currentFile_(NULL)
    , nextIndex_(0)
    , pResLoader_(NULL)
{
}

STDMETHODIMP FontResourceEnumerator::Initialize(IDWriteFactory* pFactory, IFontResourceLoader* pResLoader)
{
    if (pFactory && pResLoader)
    {
        pFactory_   = DX::SafeAcquire(pFactory);
        pResLoader_ = DX::SafeAcquire(pResLoader);
        return S_OK;
    }
    return E_INVALIDARG;
}

STDMETHODIMP FontResourceEnumerator::SetResources(const Vector<BinaryData>& data)
{
    try
    {
        resources_.assign(data.begin(), data.end());
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

HRESULT STDMETHODCALLTYPE FontResourceEnumerator::MoveNext(_Out_ BOOL* hasCurrentFile)
{
    HRESULT hr = S_OK;

    *hasCurrentFile = FALSE;
    DX::SafeRelease(currentFile_);

    if (nextIndex_ < resources_.size())
    {
        hr = pFactory_->CreateCustomFontFileReference(&resources_[nextIndex_], sizeof(BinaryData), pResLoader_,
                                                      &currentFile_);

        if (SUCCEEDED(hr))
        {
            *hasCurrentFile = TRUE;

            ++nextIndex_;
        }
    }

    return hr;
}

HRESULT STDMETHODCALLTYPE FontResourceEnumerator::GetCurrentFontFile(_Out_ IDWriteFontFile** fontFile)
{
    *fontFile = DX::SafeAcquire(currentFile_);

    return (currentFile_ != NULL) ? S_OK : E_FAIL;
}

HRESULT STDMETHODCALLTYPE FontResourceEnumerator::QueryInterface(REFIID iid, _Out_ void** ppvObject)
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

ULONG STDMETHODCALLTYPE FontResourceEnumerator::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG STDMETHODCALLTYPE FontResourceEnumerator::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount_);
    if (newCount == 0)
        delete this;

    return newCount;
}

////////////////////////////////////////////////////////////////////////////////////////
//
// FontResourceStream
//
////////////////////////////////////////////////////////////////////////////////////////

class FontResourceStream : public IFontResourceStream
{
public:
    FontResourceStream();

    STDMETHOD(Initialize)(const BinaryData& data);

    // IDWriteFontFileStream methods
    virtual HRESULT STDMETHODCALLTYPE ReadFileFragment(void const** fragmentStart, UINT64 fileOffset,
                                                       UINT64 fragmentSize, _Out_ void** fragmentContext);

    virtual void STDMETHODCALLTYPE ReleaseFileFragment(void* fragmentContext);

    virtual HRESULT STDMETHODCALLTYPE GetFileSize(_Out_ UINT64* fileSize);

    virtual HRESULT STDMETHODCALLTYPE GetLastWriteTime(_Out_ UINT64* lastWriteTime);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE   AddRef();
    virtual ULONG STDMETHODCALLTYPE   Release();

private:
    ULONG  refCount_;
    LPVOID resourcePtr_;
    DWORD  resourceSize_;
};

HRESULT IFontResourceStream::Create(_Out_ IFontResourceStream** ppStream, const BinaryData& data)
{
    HRESULT hr = S_OK;

    if (!ppStream)
    {
        hr = E_POINTER;
    }

    if (SUCCEEDED(hr))
    {
        FontResourceStream* pFileStream = new (std::nothrow) FontResourceStream;
        hr                              = pFileStream ? S_OK : E_OUTOFMEMORY;

        if (SUCCEEDED(hr))
        {
            hr = pFileStream->Initialize(data);
        }

        if (SUCCEEDED(hr))
        {
            DX::SafeRelease(*ppStream);
            (*ppStream) = DX::SafeAcquire(pFileStream);
        }
    }
    return hr;
}

FontResourceStream::FontResourceStream()
    : refCount_(0)
    , resourcePtr_(NULL)
    , resourceSize_(0)
{
}

STDMETHODIMP FontResourceStream::Initialize(const BinaryData& data)
{
    HRESULT hr = data.IsValid() ? S_OK : E_FAIL;

    if (SUCCEEDED(hr))
    {
        resourcePtr_  = data.buffer;
        resourceSize_ = data.size;
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE FontResourceStream::ReadFileFragment(void const** fragmentStart, UINT64 fileOffset,
                                                               UINT64 fragmentSize, _Out_ void** fragmentContext)
{
    // The pLoader is responsible for doing a bounds check.
    if (fileOffset <= resourceSize_ && fragmentSize <= resourceSize_ - fileOffset)
    {
        *fragmentStart   = static_cast<BYTE const*>(resourcePtr_) + static_cast<uint32_t>(fileOffset);
        *fragmentContext = NULL;
        return S_OK;
    }
    else
    {
        *fragmentStart   = NULL;
        *fragmentContext = NULL;
        return E_FAIL;
    }
}

void STDMETHODCALLTYPE FontResourceStream::ReleaseFileFragment(void* fragmentContext) {}

HRESULT STDMETHODCALLTYPE FontResourceStream::GetFileSize(_Out_ UINT64* fileSize)
{
    *fileSize = resourceSize_;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE FontResourceStream::GetLastWriteTime(_Out_ UINT64* lastWriteTime)
{
    *lastWriteTime = 0;
    return E_NOTIMPL;
}

// IUnknown methods
HRESULT STDMETHODCALLTYPE FontResourceStream::QueryInterface(REFIID iid, void** ppvObject)
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

ULONG STDMETHODCALLTYPE FontResourceStream::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG STDMETHODCALLTYPE FontResourceStream::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount_);
    if (newCount == 0)
        delete this;

    return newCount;
}

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano

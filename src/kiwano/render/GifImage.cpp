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
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <kiwano/utils/Logger.h>
#include <kiwano/render/GifImage.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

GifImagePtr GifImage::Create(const String& file_path)
{
    GifImagePtr ptr = memory::New<GifImage>();
    if (ptr)
    {
        if (!ptr->Load(file_path))
            return nullptr;
    }
    return ptr;
}

GifImagePtr GifImage::Create(const Resource& res)
{
    GifImagePtr ptr = memory::New<GifImage>();
    if (ptr)
    {
        if (!ptr->Load(res))
            return nullptr;
    }
    return ptr;
}

GifImage::GifImage()
    : frames_count_(0)
{
}

bool GifImage::Load(const String& file_path)
{
    Renderer::GetInstance().CreateGifImage(*this, file_path);

    if (IsValid())
    {
        if (GetGlobalMetadata())
            return true;

        // Clear data
        ResetNativePointer();
    }
    return false;
}

bool GifImage::Load(const Resource& res)
{
    Renderer::GetInstance().CreateGifImage(*this, res);

    if (IsValid())
    {
        if (GetGlobalMetadata())
            return true;

        // Clear data
        ResetNativePointer();
    }
    return false;
}

GifImage::Frame GifImage::GetFrame(uint32_t index)
{
    Frame frame;
    Renderer::GetInstance().CreateGifImageFrame(frame, *this, index);
    return frame;
}

}  // namespace kiwano

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#include <kiwano/render/DirectX/NativePtr.h>

namespace kiwano
{

bool GifImage::GetGlobalMetadata()
{
    ComPtr<IWICBitmapDecoder> decoder = NativePtr::Get<IWICBitmapDecoder>(this);

    HRESULT hr = decoder ? S_OK : E_FAIL;

    if (SUCCEEDED(hr))
    {
        hr = decoder->GetFrameCount(&frames_count_);
    }

    if (SUCCEEDED(hr))
    {
        ComPtr<IWICMetadataQueryReader> metadata_reader;
        hr = decoder->GetMetadataQueryReader(&metadata_reader);

        if (SUCCEEDED(hr))
        {
            uint32_t width  = 0;
            uint32_t height = 0;

            PROPVARIANT prop_val;
            ::PropVariantInit(&prop_val);

            // ��ȡȫ�� frame ��С
            if (SUCCEEDED(hr))
            {
                // ��ȡ���
                hr = metadata_reader->GetMetadataByName(L"/logscrdesc/Width", &prop_val);

                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
                    if (SUCCEEDED(hr))
                    {
                        width = prop_val.uiVal;
                    }
                    ::PropVariantClear(&prop_val);
                }
            }

            if (SUCCEEDED(hr))
            {
                // ��ȡ�߶�
                hr = metadata_reader->GetMetadataByName(L"/logscrdesc/Height", &prop_val);

                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI2 ? S_OK : E_FAIL);
                    if (SUCCEEDED(hr))
                    {
                        height = prop_val.uiVal;
                    }
                    ::PropVariantClear(&prop_val);
                }
            }

            if (SUCCEEDED(hr))
            {
                // ��������ݺ��
                hr = metadata_reader->GetMetadataByName(L"/logscrdesc/PixelAspectRatio", &prop_val);

                if (SUCCEEDED(hr))
                {
                    hr = (prop_val.vt == VT_UI1 ? S_OK : E_FAIL);
                    if (SUCCEEDED(hr))
                    {
                        if (prop_val.bVal != 0)
                        {
                            // ��Ҫ�������
                            // ������� 1��4��������� 4��1������Ϊ 1/64
                            float pixel_asp_ratio = (prop_val.bVal + 15.f) / 64.f;

                            // �������س���ȼ��������е�ͼ���Ⱥ͸߶ȣ�ֻ��Сͼ��
                            if (pixel_asp_ratio > 1.f)
                            {
                                size_in_pixels_ = { width, static_cast<uint32_t>(height / pixel_asp_ratio) };
                            }
                            else
                            {
                                size_in_pixels_ = { static_cast<uint32_t>(width * pixel_asp_ratio), height };
                            }
                        }
                        else
                        {
                            // ֵΪ 0, �������ر�Ϊ 1
                            size_in_pixels_ = { width, height };
                        }
                    }
                    ::PropVariantClear(&prop_val);
                }
            }
            ::PropVariantClear(&prop_val);
        }
    }
    return SUCCEEDED(hr);
}

#else

bool GifImage::GetGlobalMetadata()
{
    return false;  // not supported
}

#endif

}  // namespace kiwano

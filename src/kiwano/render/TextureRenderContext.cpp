// Copyright (c) 2016-2019 Kiwano - Nomango
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

#include <kiwano/core/Logger.h>
#include <kiwano/render/TextureRenderContext.h>
#include <kiwano/render/Renderer.h>

namespace kiwano
{

TextureRenderContextPtr TextureRenderContext::Create()
{
    TextureRenderContextPtr ptr = new TextureRenderContext;
    if (ptr)
    {
        try
        {
            Renderer::GetInstance().CreateTextureRenderContext(*ptr);
        }
        catch (std::exception)
        {
            return nullptr;
        }
    }
    return ptr;
}

TextureRenderContextPtr TextureRenderContext::Create(Size const& desired_size)
{
    TextureRenderContextPtr ptr = new TextureRenderContext;
    if (ptr)
    {
        try
        {
            Renderer::GetInstance().CreateTextureRenderContext(*ptr, &desired_size);
        }
        catch (std::exception)
        {
            return nullptr;
        }
    }
    return ptr;
}

TextureRenderContext::TextureRenderContext() {}

bool TextureRenderContext::GetOutput(Texture& texture)
{
    HRESULT hr = E_FAIL;

    if (bitmap_rt_)
    {
        ComPtr<ID2D1Bitmap> bitmap;

        hr = bitmap_rt_->GetBitmap(&bitmap);

        if (SUCCEEDED(hr))
        {
            texture.SetBitmap(bitmap);
        }
    }
    return SUCCEEDED(hr);
}

}  // namespace kiwano

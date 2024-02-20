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

#pragma once

#if defined(KGE_USE_DIRECTX10)
#include <kiwano/render/DirectX/D3D10DeviceResources.h>
#else
#include <kiwano/render/DirectX/D3D11DeviceResources.h>
#endif

namespace kiwano
{
namespace graphics
{
namespace directx
{

#if defined(KGE_USE_DIRECTX10)
using ID3DDeviceResources = kiwano::graphics::directx::ID3D10DeviceResources;

inline ComPtr<ID3D10DeviceResources> GetD3DDeviceResources()
{
    return kiwano::graphics::directx::GetD3D10DeviceResources();
}
#else
using ID3DDeviceResources = kiwano::graphics::directx::ID3D11DeviceResources;

inline ComPtr<ID3D11DeviceResources> GetD3DDeviceResources()
{
    return kiwano::graphics::directx::GetD3D11DeviceResources();
}
#endif

}  // namespace directx
}  // namespace graphics
}  // namespace kiwano

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

#include <kiwano/render/Shader.h>
#include <kiwano/render/Renderer.h>
#include <kiwano/platform/FileSystem.h>

#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
#include <kiwano/render/DirectX/helper.h>
#include <kiwano/render/DirectX/Effect.h>
#endif

namespace kiwano
{

PixelShader::PixelShader(const kiwano::UUID& uuid)
    : uuid_(uuid)
{
}

RefPtr<PixelShader> PixelShader::Preload(const kiwano::UUID& uuid, StringView cso_path)
{
    Vector<uint8_t> data;
    FileSystem::GetInstance().ReadFile(cso_path, data);
    return PixelShader::Preload(uuid, BinaryData(data.data(), (uint32_t)data.size()));
}

RefPtr<PixelShader> PixelShader::Preload(const kiwano::UUID& uuid, BinaryData cso_data)
{
    RefPtr<PixelShader> shader = new PixelShader(uuid);
    if (shader)
    {
        Renderer::GetInstance().CreatePixelShader(*shader, cso_data);
    }
    return shader;
}

RefPtr<Image> PixelShader::Process(RefPtr<Image> input)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto d2d_input = ComPolicy::Get<ID2D1Image>(input);
    auto effect    = ComPolicy::Get<ID2D1Effect>(this);
    effect->SetInput(0, d2d_input.Get());

    ComPtr<ID2D1Image> d2d_output;
    effect->GetOutput(&d2d_output);

    RefPtr<Image> output = new Image;
    ComPolicy::Set(output, d2d_output);
    return output;
#else
    return nullptr;
#endif
}

void PixelShader::SetConstants(const uint8_t* data, uint32_t size)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto effect = ComPolicy::Get<ID2D1Effect>(this);
    KGE_THROW_IF_FAILED(effect->SetValue(graphics::directx::CustomPixelEffect::InputIndex::Constants, data, size),
                        "ID2D1Effect SetValue failed");
#else
    // not supported
#endif
}

void PixelShader::SetExpansion(PixelSize left_top, PixelSize right_bottom)
{
#if KGE_RENDER_ENGINE == KGE_RENDER_ENGINE_DIRECTX
    auto effect = ComPolicy::Get<ID2D1Effect>(this);
    KGE_THROW_IF_FAILED(effect->SetValue(graphics::directx::CustomPixelEffect::InputIndex::LeftTopExpansion,
                                         DX::ConvertToVector2F(Size(float(left_top.x), float(left_top.y)))),
                        "ID2D1Effect SetValue failed");

    KGE_THROW_IF_FAILED(effect->SetValue(graphics::directx::CustomPixelEffect::InputIndex::RightBottomExpansion,
                                         DX::ConvertToVector2F(Size(float(right_bottom.x), float(right_bottom.y)))),
                        "ID2D1Effect SetValue failed");
#else
    // not supported
#endif
}

}  // namespace kiwano

// dear imgui: Renderer for Kiwano

#pragma once

#ifndef KGE_DOXYGEN_DO_NOT_INCLUDE

#if !defined(KGE_USE_DIRECTX10)

#include <kiwano-imgui/imgui_impl/imgui_impl_dx11.h>
#include <kiwano/render/DirectX/D3DDeviceResources.h>

inline bool ImGui_Impl_Init()
{
    auto d3d = kiwano::graphics::directx::GetD3DDeviceResources();
    return ImGui_ImplDX11_Init(d3d->GetDevice(), d3d->GetDeviceContext());
}

inline void ImGui_Impl_Shutdown()
{
    ImGui_ImplDX11_Shutdown();
}

inline void ImGui_Impl_NewFrame()
{
    ImGui_ImplDX11_NewFrame();
}

inline void ImGui_Impl_RenderDrawData(ImDrawData* draw_data)
{
    ImGui_ImplDX11_RenderDrawData(draw_data);
}

inline void ImGui_Impl_InvalidateDeviceObjects()
{
    ImGui_ImplDX11_InvalidateDeviceObjects();
}

inline bool ImGui_Impl_CreateDeviceObjects()
{
    return ImGui_ImplDX11_CreateDeviceObjects();
}

#else

#include <kiwano-imgui/imgui_impl/imgui_impl_dx10.h>
#include <kiwano/render/DirectX/D3DDeviceResources.h>

inline bool ImGui_Impl_Init()
{
    auto d3d = kiwano::graphics::directx::GetD3DDeviceResources();
    return ImGui_ImplDX10_Init(d3d->GetDevice());
}

inline void ImGui_Impl_Shutdown()
{
    ImGui_ImplDX10_Shutdown();
}

inline void ImGui_Impl_NewFrame()
{
    ImGui_ImplDX10_NewFrame();
}

inline void ImGui_Impl_RenderDrawData(ImDrawData* draw_data)
{
    ImGui_ImplDX10_RenderDrawData(draw_data);
}

inline void ImGui_Impl_InvalidateDeviceObjects()
{
    ImGui_ImplDX10_InvalidateDeviceObjects();
}

inline bool ImGui_Impl_CreateDeviceObjects()
{
    return ImGui_ImplDX10_CreateDeviceObjects();
}

#endif

#endif

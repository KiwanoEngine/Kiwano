// dear imgui: Renderer for Kiwano (DirectX11)

#pragma once
#include <imgui/imgui.h>

#ifndef KGE_DOXYGEN_DO_NOT_INCLUDE
#ifndef IMGUI_DISABLE

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11SamplerState;
struct ID3D11Buffer;

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
IMGUI_IMPL_API bool ImGui_ImplDX11_Init(ID3D11Device* device, ID3D11DeviceContext* device_context);
IMGUI_IMPL_API void ImGui_ImplDX11_Shutdown();
IMGUI_IMPL_API void ImGui_ImplDX11_NewFrame();
IMGUI_IMPL_API void ImGui_ImplDX11_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_IMPL_API bool ImGui_ImplDX11_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplDX11_InvalidateDeviceObjects();

// [BETA] Selected render state data shared with callbacks.
// This is temporarily stored in GetPlatformIO().Renderer_RenderState during the ImGui_ImplDX11_RenderDrawData() call.
// (Please open an issue if you feel you need access to more data)
struct ImGui_ImplDX11_RenderState
{
    ID3D11Device*        Device;
    ID3D11DeviceContext* DeviceContext;
    ID3D11SamplerState*  SamplerDefault;
    ID3D11Buffer*        VertexConstantBuffer;
};

#endif  // #ifndef IMGUI_DISABLE
#endif

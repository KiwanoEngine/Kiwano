// dear imgui: Renderer for Kiwano (DirectX10)

#pragma once
#include <imgui/imgui.h>

#ifndef KGE_DOXYGEN_DO_NOT_INCLUDE
#ifndef IMGUI_DISABLE

struct ID3D10Device;
struct ID3D10SamplerState;
struct ID3D10Buffer;

// Follow "Getting Started" link and check examples/ folder to learn about using backends!
IMGUI_IMPL_API bool ImGui_ImplDX10_Init(ID3D10Device* device);
IMGUI_IMPL_API void ImGui_ImplDX10_Shutdown();
IMGUI_IMPL_API void ImGui_ImplDX10_NewFrame();
IMGUI_IMPL_API void ImGui_ImplDX10_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_IMPL_API bool ImGui_ImplDX10_CreateDeviceObjects();
IMGUI_IMPL_API void ImGui_ImplDX10_InvalidateDeviceObjects();

// [BETA] Selected render state data shared with callbacks.
// This is temporarily stored in GetPlatformIO().Renderer_RenderState during the ImGui_ImplDX10_RenderDrawData() call.
// (Please open an issue if you feel you need access to more data)
struct ImGui_ImplDX10_RenderState
{
    ID3D10Device*       Device;
    ID3D10SamplerState* SamplerDefault;
    ID3D10Buffer*       VertexConstantBuffer;
};

#endif  // #ifndef IMGUI_DISABLE
#endif

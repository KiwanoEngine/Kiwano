// dear imgui: Renderer for Kiwano (DirectX10)

#pragma once
#include <imgui/imgui.h>

#ifndef KGE_DOXYGEN_DO_NOT_INCLUDE
#ifndef IMGUI_DISABLE

struct ID3D10Device;

IMGUI_IMPL_API bool ImGui_ImplDX10_Init(ID3D10Device* device);
IMGUI_IMPL_API void ImGui_ImplDX10_Shutdown();
IMGUI_IMPL_API void ImGui_ImplDX10_NewFrame();
IMGUI_IMPL_API void ImGui_ImplDX10_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing Dear ImGui state.
IMGUI_IMPL_API void ImGui_ImplDX10_InvalidateDeviceObjects();
IMGUI_IMPL_API bool ImGui_ImplDX10_CreateDeviceObjects();

#endif  // #ifndef IMGUI_DISABLE
#endif

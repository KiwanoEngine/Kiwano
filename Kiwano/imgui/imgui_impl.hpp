// dear imgui: Renderer for Kiwano

#pragma once

#if !defined(KGE_USE_DIRECTX10)

#include "imgui_impl_dx11.h"

inline bool     ImGui_Impl_Init(::kiwano::Renderer& renderer)		{ return ImGui_ImplDX11_Init(renderer.GetDeviceResources()->GetD3DDevice(), renderer.GetDeviceResources()->GetD3DDeviceContext()); }
inline void     ImGui_Impl_Shutdown()								{ ImGui_ImplDX11_Shutdown(); }
inline void     ImGui_Impl_NewFrame()								{ ImGui_ImplDX11_NewFrame(); }
inline void     ImGui_Impl_RenderDrawData(ImDrawData* draw_data)	{ ImGui_ImplDX11_RenderDrawData(draw_data); }

inline void     ImGui_Impl_InvalidateDeviceObjects()				{ ImGui_ImplDX11_InvalidateDeviceObjects(); }
inline bool     ImGui_Impl_CreateDeviceObjects()					{ return ImGui_ImplDX11_CreateDeviceObjects(); }

#else

#include "imgui_impl_dx10.h"

inline bool     ImGui_Impl_Init(::kiwano::Renderer& renderer)		{ return ImGui_ImplDX10_Init(renderer.GetDeviceResources()->GetD3DDeviceContext()); }
inline void     ImGui_Impl_Shutdown()								{ ImGui_ImplDX10_Shutdown(); }
inline void     ImGui_Impl_NewFrame()								{ ImGui_ImplDX10_NewFrame(); }
inline void     ImGui_Impl_RenderDrawData(ImDrawData* draw_data)	{ ImGui_ImplDX10_RenderDrawData(draw_data); }

inline void     ImGui_Impl_InvalidateDeviceObjects()				{ ImGui_ImplDX10_InvalidateDeviceObjects(); }
inline bool     ImGui_Impl_CreateDeviceObjects()					{ return ImGui_ImplDX10_CreateDeviceObjects(); }

#endif

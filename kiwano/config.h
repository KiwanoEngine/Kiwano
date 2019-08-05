//-----------------------------------------------------------------------------
// Compile-time options for Kiwano
//-----------------------------------------------------------------------------

#pragma once

//---- Define assertion handler. Defaults to calling assert()
//#define KGE_ASSERT(EXPR)  MyAssert(EXPR)
//#define KGE_ASSERT(EXPR)  __noop            // Disable asserts

//---- Define debug-output handler. Defaults to calling kiwano::logs::Messageln()/Warningln()/Errorln()
//#define KGE_LOG(FORMAT, ...)              wprintf(FORMAT L"\n", __VA_ARGS__)
//#define KGE_WARNING_LOG(FORMAT, ...)      wprintf(FORMAT L"\n", __VA_ARGS__)
//#define KGE_ERROR_LOG(FORMAT, ...)        wprintf(FORMAT L"\n", __VA_ARGS__)

//---- Define attributes of all API symbols declarations for DLL
//#define KGE_USE_DLL
//#define KGE_EXPORT_DLL

//---- Define DirectX version. Defaults to using Direct3D11
//#define KGE_USE_DIRECTX10

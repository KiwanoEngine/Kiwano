//-----------------------------------------------------------------------------
// Compile-time options for Easy2D
//-----------------------------------------------------------------------------

#pragma once

//---- Define assertion handler. Defaults to calling assert()
//#define E2D_ASSERT(EXPR)  MyAssert(EXPR)
//#define E2D_ASSERT(EXPR)  __noop            // Disable asserts

//---- Don't implement log functions
//#define E2D_DISABLE_LOG_FUNCTIONS

//---- Define debug-output handler. Defaults to calling easy2d::logs::Messageln()/Warningln()/Errorln()
//#define E2D_LOG(FORMAT, ...)              wprintf(FORMAT L"\n", __VA_ARGS__)
//#define E2D_WARNING_LOG(FORMAT, ...)      wprintf(FORMAT L"\n", __VA_ARGS__)
//#define E2D_ERROR_LOG(FORMAT, ...)        wprintf(FORMAT L"\n", __VA_ARGS__)

//---- Define attributes of all API symbols declarations for DLL
//#define E2D_API __declspec( dllexport )
//#define E2D_API __declspec( dllimport )

//---- Define DirectX version. Defaults to using Direct3D11
//#define E2D_USE_DIRECTX10

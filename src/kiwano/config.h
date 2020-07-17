//-----------------------------------------------------------------------------
// Compile-time configurations for Kiwano
//-----------------------------------------------------------------------------

#pragma once

//---- Define assertion handler. Defaults to calling assert()
//#define KGE_ASSERT(EXPR)  MyAssert(EXPR)
//#define KGE_ASSERT(EXPR)  __noop            // Disable asserts

//---- Define debug-output handler. Defaults to calling kiwano::logs::Messageln()/Warningln()/Errorln()
//#define KGE_LOGF(FORMAT, ...)     printf(FORMAT "\n", __VA_ARGS__)
//#define KGE_WARNF(FORMAT, ...)    printf(FORMAT "\n", __VA_ARGS__)
//#define KGE_ERRORF(FORMAT, ...)   printf(FORMAT "\n", __VA_ARGS__)

//---- Define attributes of all API symbols declarations for DLL
//#define KGE_USE_DLL
//#define KGE_EXPORT_DLL

//---- Define DirectX version. Defaults to using Direct3D11
//#define KGE_USE_DIRECTX10

//---- Define to enable DirectX debug layer
//#define KGE_ENABLE_DX_DEBUG

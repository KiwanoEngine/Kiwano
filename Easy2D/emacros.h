#pragma once


#ifndef Assert

#if defined( DEBUG ) || defined( _DEBUG )
	#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
	#define Assert(b)
#endif //DEBUG || _DEBUG

#endif

template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }
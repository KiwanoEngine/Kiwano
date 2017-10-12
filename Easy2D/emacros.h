#pragma once


#ifndef ASSERT_IF
#if defined( DEBUG ) || defined( _DEBUG )
	#define ASSERT(b) do {if (!(b)) { OutputDebugStringA("Assert: " #b "\n"); }} while(0)
#else
	#define ASSERT(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef WARN_IF
#define WARN_IF(b, m) do {if (b) { fprintf(stderr, "Warning: " #m "/n"); }} while(0)
#endif


template<typename T>
inline void SafeDelete(T** p) { if (*p) { delete *p; *p = nullptr; } }
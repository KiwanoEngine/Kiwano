// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// Website: https://www.easy2d.cn
// Source Code: https://github.com/Nomango/Easy2D
//


#pragma once

#ifndef __cplusplus
#	error 仅能在 C++ 环境下使用 Easy2D
#endif

#if _MSC_VER < 1700
#	error Easy2D 不支持 Visual Studio 2012 以下版本
#endif


#include "e2dmacros.h"
#include "e2dutil.h"
#include "e2dobject.h"
#include "e2dcomponent.h"
#include "e2dtool.h"
#include "e2daction.h"
#include "e2dtransition.h"
#include "e2dmodule.h"


#if defined(DEBUG) || defined(_DEBUG)
#	pragma comment(lib, "Easy2Ddw.lib")
#else
#	pragma comment(lib, "Easy2Dw.lib")
#endif

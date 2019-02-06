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

#pragma once
#include "RefCounter.hpp"
#include "IntrusivePtr.hpp"
#include "Array.h"
#include "closure.hpp"
#include "../math/vector.hpp"
#include "../math/Rect.hpp"
#include "../math/Matrix.h"
#include <set>
#include <map>
#include <list>
#include <unordered_set>
#include <unordered_map>


#ifndef E2D_DECLARE_SMART_PTR
#define E2D_DECLARE_SMART_PTR(class_name)\
	class class_name;\
	using class_name##Ptr = ::easy2d::IntrusivePtr< class_name >

#define E2D_DECLARE_NS_SMART_PTR(ns_name, class_name)\
	namespace ns_name\
	{\
		class class_name; \
		using class_name##Ptr = ::easy2d::IntrusivePtr< class_name >;\
	}
#endif

namespace easy2d
{
	using String = std::wstring;
	using StringStream = std::wstringstream;

	template<typename Type1, typename Type2>
	using Pair = std::pair<Type1, Type2>;

	template<typename Type>
	using List = std::list<Type>;

	template<typename Type>
	using Set = std::set<Type>;

	template<typename Type>
	using UnorderedSet = std::unordered_set<Type>;

	template<typename KeyType, typename MappedType>
	using Map = std::map<KeyType, MappedType>;

	template<typename KeyType, typename MappedType>
	using UnorderedMap = std::unordered_map<KeyType, MappedType>;
}

namespace easy2d
{
	// "Ptr" is a shorthand for "Smart Pointer"

	E2D_DECLARE_SMART_PTR(Object);
	E2D_DECLARE_SMART_PTR(Image);
	E2D_DECLARE_SMART_PTR(Music);
	E2D_DECLARE_SMART_PTR(Task);
	E2D_DECLARE_SMART_PTR(Frames);

	E2D_DECLARE_SMART_PTR(EventListener);

	E2D_DECLARE_SMART_PTR(Geometry);
	E2D_DECLARE_SMART_PTR(LineGeometry);
	E2D_DECLARE_SMART_PTR(RectangleGeometry);
	E2D_DECLARE_SMART_PTR(RoundedRectGeometry);
	E2D_DECLARE_SMART_PTR(CircleGeometry);
	E2D_DECLARE_SMART_PTR(EllipseGeometry);
	E2D_DECLARE_SMART_PTR(PathGeometry);

	E2D_DECLARE_SMART_PTR(Node);
	E2D_DECLARE_SMART_PTR(Scene);
	E2D_DECLARE_SMART_PTR(Sprite);
	E2D_DECLARE_SMART_PTR(Text);
	E2D_DECLARE_SMART_PTR(Canvas);
	E2D_DECLARE_SMART_PTR(GeometryNode);

	E2D_DECLARE_SMART_PTR(Action);
	E2D_DECLARE_SMART_PTR(ActionTween);
	E2D_DECLARE_SMART_PTR(MoveBy);
	E2D_DECLARE_SMART_PTR(MoveTo);
	E2D_DECLARE_SMART_PTR(JumpBy);
	E2D_DECLARE_SMART_PTR(JumpTo);
	E2D_DECLARE_SMART_PTR(ScaleBy);
	E2D_DECLARE_SMART_PTR(ScaleTo);
	E2D_DECLARE_SMART_PTR(OpacityBy);
	E2D_DECLARE_SMART_PTR(OpacityTo);
	E2D_DECLARE_SMART_PTR(FadeIn);
	E2D_DECLARE_SMART_PTR(FadeOut);
	E2D_DECLARE_SMART_PTR(RotateBy);
	E2D_DECLARE_SMART_PTR(RotateTo);
	E2D_DECLARE_SMART_PTR(PathAction);
	E2D_DECLARE_SMART_PTR(Delay);
	E2D_DECLARE_SMART_PTR(Animation);
	E2D_DECLARE_SMART_PTR(Loop);
	E2D_DECLARE_SMART_PTR(Sequence);
	E2D_DECLARE_SMART_PTR(Spawn);

	E2D_DECLARE_SMART_PTR(Transition);
	E2D_DECLARE_SMART_PTR(FadeTransition);
	E2D_DECLARE_SMART_PTR(EmergeTransition);
	E2D_DECLARE_SMART_PTR(BoxTransition);
	E2D_DECLARE_SMART_PTR(MoveTransition);
	E2D_DECLARE_SMART_PTR(RotationTransition);

	E2D_DECLARE_NS_SMART_PTR(ui, Button);
	E2D_DECLARE_NS_SMART_PTR(ui, Menu);

	using namespace math;
	using namespace ui;
}

namespace easy2d
{
	class __SmartPointerMaker
	{
	public:
		static inline __SmartPointerMaker const& Instance()
		{
			static __SmartPointerMaker maker;
			return maker;
		}

		template<typename T>
		inline IntrusivePtr<T> operator- (T* ptr) const
		{
			return IntrusivePtr<T>(ptr);
		}
	};
}

#ifndef E_NEW
#	define E_NEW (::easy2d::__SmartPointerMaker::Instance()) - new (std::nothrow)
#endif

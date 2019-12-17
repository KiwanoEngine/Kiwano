// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once
#include <map>
#include "vector.h"
#include "string.h"
#include "json/basic_json.h"

namespace oc
{

using json = oc::basic_json<std::map, oc::vector, oc::string, int, double, bool, std::allocator>;
using wjson = oc::basic_json<std::map, oc::vector, oc::wstring, int, double, bool, std::allocator>;

}  // namespace oc

namespace std
{

template<>
struct hash<::oc::json>
{
	size_t operator()(const ::oc::json& json) const
	{
		return hash<::oc::json::string_type>{}(json.dump());
	}
};

template<>
struct hash<::oc::wjson>
{
	size_t operator()(const ::oc::wjson& json) const
	{
		return hash<::oc::wjson::string_type>{}(json.dump());
	}
};

template<>
inline void swap<::oc::json>(::oc::json& lhs, ::oc::json& rhs) noexcept
{
	lhs.swap(rhs);
}

template<>
inline void swap<::oc::wjson>(::oc::wjson& lhs, ::oc::wjson& rhs) noexcept
{
	lhs.swap(rhs);
}

}

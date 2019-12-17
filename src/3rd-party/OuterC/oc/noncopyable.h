// Copyright (c) 2019-2020 OuterC - Nomango

#pragma once

namespace oc
{

class noncopyable
{
protected:
	noncopyable() = default;

private:
	noncopyable(const noncopyable&) = delete;

	noncopyable& operator=(const noncopyable&) = delete;
};

}  // namespace oc

#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <functional>

namespace e2d
{

struct EPoint
{
	float x;
	float y;

	EPoint()
	{
		x = 0;
		y = 0;
	}

	EPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	EPoint operator + (EPoint const & p)
	{
		return EPoint(x + p.x, y + p.y);
	}

	EPoint operator - (EPoint const & p)
	{
		return EPoint(x - p.x, y - p.y);
	}
};

struct ESize
{
	float width;
	float height;

	ESize()
	{
		width = 0;
		height = 0;
	}

	ESize(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	ESize operator + (ESize const & size)
	{
		return ESize(width + size.width, height + size.height);
	}

	ESize operator - (ESize const & size)
	{
		return ESize(width - size.width, height - size.height);
	}
};


struct EWindowStyle
{
	LPCTSTR m_pIconID;	/* 程序图标 ID */
	bool m_bNoClose;	/* 禁用关闭按钮 */
	bool m_bNoMiniSize;	/* 禁用最小化按钮 */
	bool m_bTopMost;	/* 窗口置顶 */

	EWindowStyle()
	{
		m_pIconID = 0;
		m_bNoClose = false;
		m_bNoMiniSize = false;
		m_bTopMost = false;
	}

	EWindowStyle(
		LPCTSTR pIconID
	)
	{
		m_pIconID = pIconID;
		m_bNoClose = false;
		m_bNoMiniSize = false;
		m_bTopMost = false;
	}

	EWindowStyle(
		LPCTSTR pIconID,
		bool bNoClose,
		bool bNoMiniSize,
		bool bTopMost
	)
	{
		m_pIconID = pIconID;
		m_bNoClose = bNoClose;
		m_bNoMiniSize = bNoMiniSize;
		m_bTopMost = bTopMost;
	}
};

typedef EPoint EVec;

typedef std::wstring EString;

template<typename T>
using EVector = std::vector<T>;

// 定时器回调函数（参数为该定时器被调用的次数，从 0 开始）
typedef std::function<void(int)> TIMER_CALLBACK;

// 按钮点击回调函数
typedef std::function<void()> BUTTON_CLICK_CALLBACK;

// 按键消息监听回调函数
typedef std::function<void()> KEY_LISTENER_CALLBACK;

// 鼠标消息监听回调函数
typedef std::function<void()> MOUSE_LISTENER_CALLBACK;

// 鼠标点击消息监听回调函数（参数为点击位置）
typedef std::function<void(EPoint mousePos)> MOUSE_CLICK_LISTENER_CALLBACK;

// 鼠标按下消息监听回调函数（参数为按下位置）
typedef MOUSE_CLICK_LISTENER_CALLBACK MOUSE_PRESS_LISTENER_CALLBACK;

// 鼠标双击消息监听回调函数（参数为双击位置）
typedef MOUSE_CLICK_LISTENER_CALLBACK MOUSE_DBLCLK_LISTENER_CALLBACK;

// 鼠标拖动消息监听回调函数（参数为拖动前位置和拖动后位置）
typedef std::function<void(EPoint begin, EPoint end)> MOUSE_DRAG_LISTENER_CALLBACK;

}
#include "..\easy2d.h"
#include "..\EasyX\easyx.h"
#include <conio.h>

// 按键监听回调函数的容器
static std::vector<KeyMsg*> s_vKeyMsg;

// 虚拟键值的容器
static std::vector<VK_KEY> s_vKeys = {
	KeyMsg::A, KeyMsg::B, KeyMsg::C, KeyMsg::D, KeyMsg::E, KeyMsg::F, KeyMsg::G, KeyMsg::H, KeyMsg::I, KeyMsg::J,
	KeyMsg::K, KeyMsg::L, KeyMsg::M, KeyMsg::N, KeyMsg::O, KeyMsg::P, KeyMsg::Q, KeyMsg::R, KeyMsg::S, KeyMsg::T,
	KeyMsg::U, KeyMsg::V, KeyMsg::W, KeyMsg::X, KeyMsg::Y, KeyMsg::Z,
	KeyMsg::NUM_1, KeyMsg::NUM_2, KeyMsg::NUM_3, KeyMsg::NUM_4, KeyMsg::NUM_5,
	KeyMsg::NUM_6, KeyMsg::NUM_7, KeyMsg::NUM_8, KeyMsg::NUM_9, KeyMsg::NUM_0,
	KeyMsg::NUMPAD_1, KeyMsg::NUMPAD_2, KeyMsg::NUMPAD_3, KeyMsg::NUMPAD_4, KeyMsg::NUMPAD_5,
	KeyMsg::NUMPAD_6, KeyMsg::NUMPAD_7, KeyMsg::NUMPAD_8, KeyMsg::NUMPAD_9, KeyMsg::NUMPAD_0,
	KeyMsg::Enter, KeyMsg::Space, KeyMsg::Up, KeyMsg::Down, KeyMsg::Left, KeyMsg::Right, KeyMsg::Esc,
	KeyMsg::Decimal, KeyMsg::Shift, KeyMsg::LShift, KeyMsg::RShift, KeyMsg::Ctrl, KeyMsg::LCtrl, KeyMsg::RCtrl,
	KeyMsg::F1, KeyMsg::F2, KeyMsg::F3, KeyMsg::F4, KeyMsg::F5, KeyMsg::F6,
	KeyMsg::F7, KeyMsg::F8, KeyMsg::F9, KeyMsg::F10, KeyMsg::F11, KeyMsg::F12
};

// 虚拟键值的定义
const VK_KEY KeyMsg::A = 'A';
const VK_KEY KeyMsg::B = 'B';
const VK_KEY KeyMsg::C = 'C';
const VK_KEY KeyMsg::D = 'D';
const VK_KEY KeyMsg::E = 'E';
const VK_KEY KeyMsg::F = 'F';
const VK_KEY KeyMsg::G = 'G';
const VK_KEY KeyMsg::H = 'H';
const VK_KEY KeyMsg::I = 'I';
const VK_KEY KeyMsg::J = 'J';
const VK_KEY KeyMsg::K = 'K';
const VK_KEY KeyMsg::L = 'L';
const VK_KEY KeyMsg::M = 'M';
const VK_KEY KeyMsg::N = 'N';
const VK_KEY KeyMsg::O = 'O';
const VK_KEY KeyMsg::P = 'P';
const VK_KEY KeyMsg::Q = 'Q';
const VK_KEY KeyMsg::R = 'R';
const VK_KEY KeyMsg::S = 'S';
const VK_KEY KeyMsg::T = 'T';
const VK_KEY KeyMsg::U = 'U';
const VK_KEY KeyMsg::V = 'V';
const VK_KEY KeyMsg::W = 'W';
const VK_KEY KeyMsg::X = 'X';
const VK_KEY KeyMsg::Y = 'Y';
const VK_KEY KeyMsg::Z = 'Z';
const VK_KEY KeyMsg::NUM_0 = '0';
const VK_KEY KeyMsg::NUM_1 = '1';
const VK_KEY KeyMsg::NUM_2 = '2';
const VK_KEY KeyMsg::NUM_3 = '3';
const VK_KEY KeyMsg::NUM_4 = '4';
const VK_KEY KeyMsg::NUM_5 = '5';
const VK_KEY KeyMsg::NUM_6 = '6';
const VK_KEY KeyMsg::NUM_7 = '7';
const VK_KEY KeyMsg::NUM_8 = '8';
const VK_KEY KeyMsg::NUM_9 = '9';
const VK_KEY KeyMsg::NUMPAD_0 = VK_NUMPAD0;
const VK_KEY KeyMsg::NUMPAD_1 = VK_NUMPAD1;
const VK_KEY KeyMsg::NUMPAD_2 = VK_NUMPAD2;
const VK_KEY KeyMsg::NUMPAD_3 = VK_NUMPAD3;
const VK_KEY KeyMsg::NUMPAD_4 = VK_NUMPAD4;
const VK_KEY KeyMsg::NUMPAD_5 = VK_NUMPAD5;
const VK_KEY KeyMsg::NUMPAD_6 = VK_NUMPAD6;
const VK_KEY KeyMsg::NUMPAD_7 = VK_NUMPAD7;
const VK_KEY KeyMsg::NUMPAD_8 = VK_NUMPAD8;
const VK_KEY KeyMsg::NUMPAD_9 = VK_NUMPAD9;
const VK_KEY KeyMsg::Enter = VK_RETURN;
const VK_KEY KeyMsg::Space = VK_SPACE;
const VK_KEY KeyMsg::Decimal = VK_DECIMAL;
const VK_KEY KeyMsg::Ctrl = VK_CONTROL;
const VK_KEY KeyMsg::LCtrl = VK_LCONTROL;
const VK_KEY KeyMsg::RCtrl = VK_RCONTROL;
const VK_KEY KeyMsg::Shift = VK_SHIFT;
const VK_KEY KeyMsg::LShift = VK_LSHIFT;
const VK_KEY KeyMsg::RShift = VK_RSHIFT;
const VK_KEY KeyMsg::Up = VK_UP;
const VK_KEY KeyMsg::Down = VK_DOWN;
const VK_KEY KeyMsg::Left = VK_LEFT;
const VK_KEY KeyMsg::Right = VK_RIGHT;
const VK_KEY KeyMsg::Esc = VK_ESCAPE;
const VK_KEY KeyMsg::F1 = VK_F1;
const VK_KEY KeyMsg::F2 = VK_F2;
const VK_KEY KeyMsg::F3 = VK_F3;
const VK_KEY KeyMsg::F4 = VK_F4;
const VK_KEY KeyMsg::F5 = VK_F5;
const VK_KEY KeyMsg::F6 = VK_F6;
const VK_KEY KeyMsg::F7 = VK_F7;
const VK_KEY KeyMsg::F8 = VK_F8;
const VK_KEY KeyMsg::F9 = VK_F9;
const VK_KEY KeyMsg::F10 = VK_F10;
const VK_KEY KeyMsg::F11 = VK_F11;
const VK_KEY KeyMsg::F12 = VK_F12;

KeyMsg::KeyMsg(tstring name, const KEY_CALLBACK & callback)
{
	m_sName = name;
	m_callback = callback;
}

KeyMsg::~KeyMsg()
{
}

void KeyMsg::onKbHit(VK_KEY key)
{
	m_callback(key);
}

void KeyMsg::__exec()
{
	if (_kbhit())								// 获取键盘消息
	{
		for (VK_KEY key : s_vKeys)				// 循环遍历所有的虚拟键值
		{
			if (GetAsyncKeyState(key) & 0x8000)	// 判断该键是否按下
			{
				for (auto k : s_vKeyMsg)		// 分发该按键消息
				{
					k->onKbHit(key);			// 执行按键回调函数
				}
			}
		}
	}
}

void KeyMsg::addListener(tstring name, const KEY_CALLBACK & callback)
{
	// 创建新的监听对象
	auto key = new KeyMsg(name, callback);
	// 添加新的按键回调函数
	s_vKeyMsg.push_back(key);
}

bool KeyMsg::delListener(tstring name)
{
	// 创建迭代器
	std::vector<KeyMsg*>::iterator iter;
	// 循环遍历所有监听器
	for (iter = s_vKeyMsg.begin(); iter != s_vKeyMsg.end(); iter++)
	{
		// 查找相同名称的监听器
		if ((*iter)->m_sName == name)
		{
			// 删除该定时器
			delete (*iter);
			s_vKeyMsg.erase(iter);
			return true;
		}
	}
	// 若未找到同样名称的监听器，返回 false
	return false;
}

void KeyMsg::clearAllListener()
{
	// 删除所有监听器
	for (auto k : s_vKeyMsg)
	{
		delete k;
	}
	// 清空容器
	s_vKeyMsg.clear();
}

bool KeyMsg::isKeyDown(VK_KEY key)
{
	// 获取 key 的按下情况
	return (GetAsyncKeyState(key) & 0x8000);
}
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
#include "base.hpp"
#include "time.h"
#include "window.h"
#include "render.h"
#include "input.h"
#include "audio.h"
#include "Event.hpp"

namespace easy2d
{
	struct Options
	{
		String	title;	// ����
		int		width;	// ���
		int		height;	// �߶�
		LPCWSTR	icon;	// ͼ��
		bool	vsync;	// ��ֱͬ��
		bool	debug;	// ����ģʽ

		Options()
			: title(L"Easy2D Game")
			, width(640)
			, height(480)
			, icon(nullptr)
			, vsync(true)
			, debug(false)
		{}
	};


	class Game
		: protected Noncopyable
	{
	public:
		Game();

		Game(
			Options const& options
		);

		virtual ~Game();

		// �˳���Ϸ
		virtual void OnExit() {}

		// ���ڹر�
		virtual bool OnClose() { return true; }

		// ��ʼ��
		void Init(
			Options const& options
		);

		// ����
		void Run();

		// ����
		void Quit();

		// �л�����
		void EnterScene(
			spScene const& scene			/* ���� */
		);

		// �л�����
		void EnterScene(
			spScene const& scene,			/* ���� */
			spTransition const& transition	/* �������� */
		);

		// ��ȡ��ǰ����
		spScene const& GetCurrentScene();

		// ���ñ���
		void SetTimeScale(float scale);

	private:
		void Render(HWND);

		void Update();

		bool HandleMessage(
			HWND hwnd,
			UINT msg,
			WPARAM wparam,
			LPARAM lparam
		);

		void Dispatch(
			Event* event
		);

		static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

	private:
		bool			debug_;
		bool			active_;
		float			time_scale_;
		spScene			curr_scene_;
		spScene			next_scene_;
		spTransition	transition_;
	};
}

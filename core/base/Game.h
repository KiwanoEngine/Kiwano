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
#include "base.h"
#include "window.h"

namespace easy2d
{
	class Scene;
	class Transition;

	struct Options
	{
		String	title;	/* ���� */
		int		width;	/* ��� */
		int		height;	/* �߶� */
		LPCWSTR	icon;	/* ͼ�� */
		bool	debug;	/* ����ģʽ */

		Options()
			: title(L"Easy2D Game")
			, width(640)
			, height(480)
			, icon(nullptr)
			, debug(false)
		{}
	};


	class Game
	{
		E2D_DISABLE_COPY(Game);

	public:
		Game();

		virtual ~Game();

		// ����ʱ
		virtual void OnUpdate(float dt) {}

		// �˳�ʱ
		virtual void OnExit() {}

		// ���ڹر�ʱ
		// ����ֵ������ false ����ֹ���ڹر�
		virtual bool OnClose() { return true; }

		// ��ʼ��
		void Initialize(
			const Options& options	/* ���� */
		);

		// ����
		void Run();

		// ����
		void Quit();

		// �л�����
		void EnterScene(
			Scene * scene,						/* ���� */
			Transition * transition	= nullptr	/* �������� */
		);

		// ��ȡ��ǰ����
		Scene * GetCurrentScene();

		// �Ƿ����ڽ��г�������
		bool IsTransitioning() const;

		// ��Ⱦ��������
		void DrawScene();

		// ���³���
		void UpdateScene(
			float dt
		);

	private:
		bool		debug_mode_;
		bool		quit_;
		Scene*		curr_scene_;
		Scene*		next_scene_;
		Transition*	transition_;
	};
}

// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"

class Demo5
	: public Scene
{
public:
	static ScenePtr Create()
	{
		return new Demo5;
	}

	Demo5()
	{
		AddListener(Event::KeyDown, Closure(this, &Demo5::OnKeyDown));

		// 创建说明文字
		TextPtr text = new Text(L"按空格键发送测试请求");
		// 设置节点大小为文字布局大小
		text->SetSize(text->GetLayoutSize());
		// 设置文字位置
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// 添加到场景
		this->AddChild(text);
	}

	void OnEnter() override
	{
		Application::ShowConsole(true);
	}

	void OnExit() override
	{
		Application::ShowConsole(false);
	}

	void OnKeyDown(Event const& e)
	{
		if (e.key.code == KeyCode::Space)
		{
			HttpRequestPtr request = new HttpRequest;
			request->SetUrl(L"http://httpbin.org/get/");
			request->SetType(HttpRequest::Type::Get);
			request->SetResponseCallback(Closure(this, &Demo5::Complete));

			HttpClient::Instance().Send(request);
		}
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		if (response->IsSucceed())
		{
			std::wcout << "Response: " << std::endl << "HttpCode: " << response->GetResponseCode()
				<< std::endl << "Data: " << response->GetData() << std::endl;
		}
		else
		{
			std::wcout << "Error: " << response->GetError() << std::endl;
		}
	}
};

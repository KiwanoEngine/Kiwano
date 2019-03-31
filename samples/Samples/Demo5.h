// Copyright (C) 2019 Nomango

#pragma once
#include "common.h"
#include <iostream>

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
		SendRequest();
	}

	void OnExit() override
	{
		Application::ShowConsole(false);
	}

	void OnKeyDown(Event const& e)
	{
		if (e.key.code == KeyCode::Space)
		{
			SendRequest();
		}
	}

	void SendRequest()
	{
		Logger::Instance().Println(L"Start to send request...");

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/get");
		request->SetType(HttpRequest::Type::Get);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		if (response->IsSucceed())
		{
			Json response_data = Json::parse(response->GetData());
			Json result = {
				{L"HttpCode", response->GetResponseCode()},
				{L"Data", response_data},
			};
			std::wcout << L"Response: " << std::endl << result.dump(4) << std::endl;
		}
		else
		{
			Json result = {
				{L"HttpCode", response->GetResponseCode()},
				{L"Error", response->GetError()},
			};
			std::wcout << L"Response: " << std::endl << result.dump(4) << std::endl;
		}
	}
};

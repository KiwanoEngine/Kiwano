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

		// ����˵������
		TextPtr text = new Text(L"���ո�����Ͳ�������");
		// ���ýڵ��СΪ���ֲ��ִ�С
		text->SetSize(text->GetLayoutSize());
		// ��������λ��
		text->SetAnchor(0.5f, 0.5f);
		text->SetPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

		// ��ӵ�����
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

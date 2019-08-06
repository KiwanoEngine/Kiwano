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

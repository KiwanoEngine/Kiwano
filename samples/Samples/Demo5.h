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
		// 添加按键监听
		AddListener(Event::KeyDown, Closure(this, &Demo5::OnKeyDown));

		// 创建说明文字
		TextPtr text = new Text(L"按G发送GET请求\n按P发送POST请求\n按U发送PUT请求\n按D发送DELETE请求");
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
		// 进入场景时打开控制台
		Application::ShowConsole(true);
	}

	void OnExit() override
	{
		// 退出场景时关闭控制台
		Application::ShowConsole(false);
	}

	void OnKeyDown(Event const& e)
	{
		// 按不同键发送不同请求
		if (e.key.code == KeyCode::G)
		{
			SendGetRequest();
		}
		else if (e.key.code == KeyCode::P)
		{
			SendPostRequest();
		}
		else if (e.key.code == KeyCode::U)
		{
			SendPutRequest();
		}
		else if (e.key.code == KeyCode::D)
		{
			SendDeleteRequest();
		}
	}

	void SendGetRequest()
	{
		// 发送 GET 请求
		Logger::Instance().Println(L"Start to send GET request...");

		HttpRequestPtr request = new HttpRequest;
		// 设置请求 URL
		request->SetUrl(L"http://httpbin.org/get");
		// 设置请求类型为 GET
		request->SetType(HttpRequest::Type::Get);
		// 设置请求完成后的回调函数
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		// 发送 HTTP 请求
		HttpClient::Instance().Send(request);
	}

	void SendPostRequest()
	{
		// 发送 POST 请求
		Logger::Instance().Println(L"Start to send POST request...");

		// 创建 JSON 格式的 POST 数据
		Json request_data = {
			{"String", "StringTest"},
			{"Boolean", true},
			{"Integer", 12},
			{"Float", 3.125},
			{"Array", {1, 2, 3, 4, 4.5 }},
			{"Object", {"Key", "Value"}},
		};

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/post");
		request->SetType(HttpRequest::Type::Post);
		// 设置 POST 请求的数据
		request->SetData(request_data.dump());
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void SendPutRequest()
	{
		// 发送 PUT 请求
		Logger::Instance().Println(L"Start to send PUT request...");

		// 创建 JSON 格式的 PUT 数据
		Json request_data = {
			{"user", {{"id", 1}, {"phone", "12345678"}}},
		};

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/put");
		request->SetType(HttpRequest::Type::Put);
		// 设置 PUT 请求的数据
		request->SetData(request_data.dump());
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void SendDeleteRequest()
	{
		// 发送 DELETE 请求
		Logger::Instance().Println(L"Start to send DELETE request...");

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/delete");
		request->SetType(HttpRequest::Type::Delete);
		request->SetResponseCallback(Closure(this, &Demo5::Complete));

		HttpClient::Instance().Send(request);
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		// 判断请求是否成功
		if (response->IsSucceed())
		{
			try
			{
				// 将获取到的数据解析成 JSON 格式
				Json response_data = Json::parse(response->GetData());
				Json result = {
					{L"HttpCode", response->GetResponseCode()},
					{L"Data", response_data},
				};

				std::wcout << L"Response: " << std::endl << result.dump(4) << std::endl;
			}
			catch (json_exception& e)
			{
				E2D_ERROR_LOG(L"Parse JSON failed: %s", e.what());
			}
		}
		else
		{
			// 请求失败时打印错误信息
			OutputError(response);
		}
	}

	void OutputError(HttpResponsePtr response)
	{
		// 打印 HTTP 响应结果的状态码和错误信息
		Json result = {
				{L"HttpCode", response->GetResponseCode()},
				{L"Error", response->GetError()},
		};
		std::wcout << L"Response: " << std::endl << result.dump(4) << std::endl;
	}
};

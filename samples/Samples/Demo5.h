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
		// ��Ӱ�������
		AddListener(Event::KeyDown, MakeClosure(this, &Demo5::OnKeyDown));

		// ����˵������
		TextPtr text = new Text(L"��G����GET����\n��P����POST����\n��U����PUT����\n��D����DELETE����");
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
		// ���볡��ʱ�򿪿���̨
		Logger::Instance()->ShowConsole(true);
	}

	void OnExit() override
	{
		// �˳�����ʱ�رտ���̨
		Logger::Instance()->ShowConsole(false);
	}

	void OnKeyDown(Event const& e)
	{
		// ����ͬ�����Ͳ�ͬ����
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
		// ���� GET ����
		Logger::Instance()->Println(L"Start to send GET request...");

		HttpRequestPtr request = new HttpRequest;
		// �������� URL
		request->SetUrl(L"http://httpbin.org/get");
		// ������������Ϊ GET
		request->SetType(HttpRequest::Type::Get);
		// ����������ɺ�Ļص�����
		request->SetResponseCallback(MakeClosure(this, &Demo5::Complete));

		// ���� HTTP ����
		HttpClient::Instance()->Send(request);
	}

	void SendPostRequest()
	{
		// ���� POST ����
		Logger::Instance()->Println(L"Start to send POST request...");

		// ���� JSON ��ʽ�� POST ����
		Json request_data = {
			{ L"string",	L"test����" },
			{ L"boolean",	true },
			{ L"integer",	12 },
			{ L"float",		3.125 },
			{ L"array",		{ 1, 2, 3, 4, 4.5 } },
			{ L"object",	{ L"key", L"value" } },
		};

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/post");
		request->SetType(HttpRequest::Type::Post);
		// ���� POST ���������
		request->SetJsonData(request_data);
		request->SetResponseCallback(MakeClosure(this, &Demo5::Complete));

		HttpClient::Instance()->Send(request);
	}

	void SendPutRequest()
	{
		// ���� PUT ����
		Logger::Instance()->Println(L"Start to send PUT request...");

		// ���� JSON ��ʽ�� PUT ����
		Json request_data = Json::array({ 1, 2, 3 });

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/put");
		request->SetType(HttpRequest::Type::Put);
		// ���� PUT ���������
		request->SetJsonData(request_data);
		request->SetResponseCallback(MakeClosure(this, &Demo5::Complete));

		HttpClient::Instance()->Send(request);
	}

	void SendDeleteRequest()
	{
		// ���� DELETE ����
		Logger::Instance()->Println(L"Start to send DELETE request...");

		HttpRequestPtr request = new HttpRequest;
		request->SetUrl(L"http://httpbin.org/delete");
		request->SetType(HttpRequest::Type::Delete);
		request->SetResponseCallback(MakeClosure(this, &Demo5::Complete));

		HttpClient::Instance()->Send(request);
	}

	void Complete(HttpRequestPtr request, HttpResponsePtr response)
	{
		// �ж������Ƿ�ɹ�
		if (response->IsSucceed())
		{
			try
			{
				// ����ȡ�������ݽ����� JSON ��ʽ
				Json response_data = Json::parse(response->GetData());
				Json result = {
					{L"HttpCode", response->GetResponseCode()},
					{L"Data", response_data},
				};

				std::wcout << L"Response: " << std::endl << result.dump(4) << std::endl;
			}
			catch (json_exception& e)
			{
				std::wcout << L"Parse JSON failed: " << e.what() << std::endl;
			}
		}
		else
		{
			// ����ʧ��ʱ��ӡ������Ϣ
			OutputError(response);
		}
	}

	void OutputError(HttpResponsePtr response)
	{
		// ��ӡ HTTP ��Ӧ�����״̬��ʹ�����Ϣ
		Json result = {
				{L"HttpCode", response->GetResponseCode()},
				{L"Error", response->GetError()},
		};
		std::wcout << L"Response: " << std::endl << result.dump(4) << std::endl;
	}
};

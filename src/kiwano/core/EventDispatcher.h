// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include <kiwano/core/EventListener.h>

namespace kiwano
{
	/**
	* \~chinese
	* @brief �¼��ַ�ϵͳ
	*/
	class KGE_API EventDispatcher
	{
	public:
		/// \~chinese
		/// @brief �������б�
		using Listeners = IntrusiveList<EventListenerPtr>;

		/// \~chinese
		/// @brief ���Ӽ�����
		EventListener* AddListener(EventListenerPtr listener);

		/// \~chinese
		/// @brief ���Ӽ�����
		EventListener* AddListener(EventListener* listener);

		/// \~chinese
		/// @brief ���Ӽ�����
		/// @param type �������¼�����
		/// @param callback �ص�����
		EventListener* AddListener(EventType type, EventListener::Callback callback);

		/// \~chinese
		/// @brief ���Ӽ�����
		/// @param name ����������
		/// @param type �������¼�����
		/// @param callback �ص�����
		EventListener* AddListener(String const& name, EventType type, EventListener::Callback callback);

		/// \~chinese
		/// @brief ���Ӽ�����
		/// @tparam _EventTy �¼�����
		/// @param callback �ص�����
		template <
			typename _EventTy,
			typename = typename std::enable_if<IsEvent<_EventTy>::value, int>::type
		>
		EventListener* AddListener(EventListener::Callback callback)
		{
			return AddListener(KGE_EVENT(_EventTy), callback);
		}

		/// \~chinese
		/// @brief ���Ӽ�����
		/// @tparam _EventTy �¼�����
		/// @param name ����������
		/// @param callback �ص�����
		template <
			typename _EventTy,
			typename = typename std::enable_if<IsEvent<_EventTy>::value, int>
		>
		EventListener* AddListener(String const& name, EventListener::Callback callback)
		{
			return AddListener(name, KGE_EVENT(_EventTy), callback);
		}

		/// \~chinese
		/// @brief ����������
		/// @param name ����������
		void StartListeners(String const& name);

		/// \~chinese
		/// @brief ֹͣ������
		/// @param name ����������
		void StopListeners(String const& name);

		/// \~chinese
		/// @brief �Ƴ�������
		/// @param name ����������
		void RemoveListeners(String const& name);

		/// \~chinese
		/// @brief ����������
		/// @param type �������¼�����
		void StartListeners(const EventType& type);

		/// \~chinese
		/// @brief ֹͣ������
		/// @param type �������¼�����
		void StopListeners(const EventType& type);

		/// \~chinese
		/// @brief �Ƴ�������
		/// @param type �������¼�����
		void RemoveListeners(const EventType& type);

		/// \~chinese
		/// @brief �������м�����
		void StartAllListeners();

		/// \~chinese
		/// @brief ֹͣ���м�����
		void StopAllListeners();

		/// \~chinese
		/// @brief �Ƴ����м�����
		void RemoveAllListeners();

		/// \~chinese
		/// @brief ��ȡ���м�����
		const Listeners& GetAllListeners() const;

		/// \~chinese
		/// @brief �ַ��¼�
		/// @param evt �¼�
		virtual void Dispatch(Event& evt);

	private:
		Listeners listeners_;
	};
}

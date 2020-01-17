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
#include <kiwano/2d/Stage.h>
#include <kiwano/render/LayerArea.h>

namespace kiwano
{
	class Director;
	class RenderContext;

	KGE_DECLARE_SMART_PTR(Transition);
	KGE_DECLARE_SMART_PTR(FadeTransition);
	KGE_DECLARE_SMART_PTR(EmergeTransition);
	KGE_DECLARE_SMART_PTR(BoxTransition);
	KGE_DECLARE_SMART_PTR(MoveTransition);
	KGE_DECLARE_SMART_PTR(RotationTransition);

	/**
	* \~chinese
	* @brief ��̨���ɶ���
	*/
	class KGE_API Transition
		: public virtual ObjectBase
	{
		friend class Director;

	public:
		/**
		* \~chinese
		* @brief �����յĳ������ɶ���
		* @param duration ����ʱ��
		*/
		explicit Transition(
			Duration duration
		);

		virtual ~Transition();

		/**
		* \~chinese
		* @brief �������ɶ����Ƿ��ѽ���
		*/
		bool IsDone();

	protected:
		/**
		* \~chinese
		* @brief ��ʼ���������ɶ���
		* @param[in] prev ת������
		* @param[in] next ת�볡��
		*/
		virtual void Init(
			StagePtr prev,
			StagePtr next
		);

		/**
		* \~chinese
		* @brief ���¹��ɶ���
		* @param dt ����һ�θ��µ�ʱ����
		*/
		virtual void Update(Duration dt);

		/**
		* \~chinese
		* @brief ��Ⱦ���ȶ���
		* @param[in] ctx ��Ⱦ������
		*/
		virtual void Render(RenderContext& ctx);

		/**
		* \~chinese
		* @brief ֹͣ����
		*/
		virtual void Stop();

		/**
		* \~chinese
		* @brief ���ö���
		*/
		virtual void Reset() {}

	protected:
		bool		done_;
		float		process_;
		Duration	duration_;
		Duration	delta_;
		Size		window_size_;
		StagePtr	out_stage_;
		StagePtr	in_stage_;
		LayerArea	out_layer_;
		LayerArea	in_layer_;
	};


	/**
	* \~chinese
	* @brief ���뵭�����ɶ���
	* @details ǰһ�����������������󣬺�һ��������
	*/
	class FadeTransition
		: public Transition
	{
	public:
		/**
		* \~chinese
		* @brief �������뵭�����ɶ���
		* @param duration ����ʱ��
		*/
		explicit FadeTransition(
			Duration duration
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;
	};


	/**
	* \~chinese
	* @brief ������ɶ���
	* @details ǰһ��������������ͬʱ����һ��������
	*/
	class EmergeTransition
		: public Transition
	{
	public:
		/**
		* \~chinese
		* @brief ����������ɶ���
		* @param duration ����ʱ��
		*/
		explicit EmergeTransition(
			Duration duration
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;
	};


	/**
	* \~chinese
	* @brief ��״���ɶ���
	* @details ǰһ�����Ժ�״��������ʧ����һ�����Ժ�״����
	*/
	class BoxTransition
		: public Transition
	{
	public:
		/**
		* \~chinese
		* @brief ������״���ɶ���
		* @param duration ����ʱ��
		*/
		explicit BoxTransition(
			Duration duration
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;
	};


	/**
	* \~chinese
	* @brief λ�ƹ��ɶ���
	* @details ��������λ�Ƶķ�ʽ�л�
	*/
	class MoveTransition
		: public Transition
	{
	public:
		/**
		* \~chinese
		* @brief λ�Ʒ�ʽ
		*/
		enum class Type : int
		{
			Up,		///< ����
			Down,	///< ����
			Left,	///< ����
			Right	///< ����
		};

		/**
		* \~chinese
		* @brief λ�ƹ��ɶ���
		* @param duration ����ʱ��
		* @param type λ�Ʒ�ʽ
		*/
		explicit MoveTransition(
			Duration duration,
			Type type
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;

		void Reset() override;

	private:
		Type	type_;
		Point	pos_delta_;
		Point	start_pos_;
	};


	/**
	* \~chinese
	* @brief ��ת���ɶ���
	* @details ǰһ��������ת��ʽ��������ʧ����һ��������ת��ʽ����
	*/
	class RotationTransition
		: public Transition
	{
	public:
		/**
		* \~chinese
		* @brief ������ת���ɶ���
		* @param duration ����ʱ��
		* @param rotation ��ת����
		*/
		explicit RotationTransition(
			Duration duration,
			float rotation = 360
		);

	protected:
		void Update(Duration dt) override;

		virtual void Init(
			StagePtr prev,
			StagePtr next
		) override;

		void Reset() override;

	private:
		float	rotation_;
	};
}

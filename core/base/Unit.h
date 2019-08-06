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
#include "Transform.hpp"

namespace easy2d
{
	class Unit
		: public ObjectBase
	{
	public:
		Unit();

		virtual ~Unit();

		// ��ȡ����
		virtual const Point& GetPosition() const { return transform_.position; }

		// ��ȡ�������ű���
		virtual float GetScaleX() const { return transform_.scale.x; }

		// ��ȡ�������ű���
		virtual float GetScaleY() const { return transform_.scale.y; }

		// ��ȡ������нǶ�
		virtual float GetSkewX() const { return transform_.skew.x; }

		// ��ȡ������нǶ�
		virtual float GetSkewY() const { return transform_.skew.y; }

		// ��ȡ��ת�Ƕ�
		virtual float GetRotation() const { return transform_.rotation; }

		Transform const& GetTransform() const { return transform_; }

		// ���ú�����
		void SetPositionX(
			float x
		);

		// ����������
		void SetPositionY(
			float y
		);

		// ��������
		void SetPosition(
			const Point & point
		);

		// ��������
		virtual void SetPosition(
			float x,
			float y
		);

		// �ƶ�
		void MoveBy(
			float x,
			float y
		);

		// �ƶ�
		void MoveBy(
			const Point & vector
		);

		// ���ú������ű���
		// Ĭ��Ϊ 1.0
		void SetScaleX(
			float scale_x
		);

		// �����������ű���
		// Ĭ��Ϊ 1.0
		void SetScaleY(
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ (1.0, 1.0)
		virtual void SetScale(
			float scale_x,
			float scale_y
		);

		// �������ű���
		// Ĭ��Ϊ 1.0
		void SetScale(
			float scale
		);

		// ���ú�����нǶ�
		// Ĭ��Ϊ 0
		void SetSkewX(
			float skew_x
		);

		// ����������нǶ�
		// Ĭ��Ϊ 0
		void SetSkewY(
			float skew_y
		);

		// ���ô��нǶ�
		// Ĭ��Ϊ (0, 0)
		virtual void SetSkew(
			float skew_x,
			float skew_y
		);

		// ������ת�Ƕ�
		// Ĭ��Ϊ 0
		virtual void SetRotation(
			float rotation
		);

		virtual void SetTransform(
			Transform const& transform
		);

		// ��ȡ��ά�任����
		virtual math::Matrix const& GetTransformMatrix();

	protected:
		virtual void UpdateMatrix();

	protected:
		bool			dirty_transform_;
		Transform		transform_;
		math::Matrix	matrix_cached_;
	};
}

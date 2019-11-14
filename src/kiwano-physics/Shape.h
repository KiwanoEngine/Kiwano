// Copyright (c) 2018-2019 Kiwano - Nomango
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
#include <kiwano-physics/helper.h>

namespace kiwano
{
	namespace physics
	{
		class World;

		// ��״����
		class KGE_API Shape
		{
		public:
			Shape();
			Shape(b2Shape* shape);

			b2Shape* GetB2Shape();
			const b2Shape* GetB2Shape() const;
			void SetB2Shape(b2Shape* shape);

			virtual void FitWorld(World* world) {}

		protected:
			b2Shape* shape_;
		};

		// Բ����״
		class KGE_API CircleShape
			: public Shape
		{
		public:
			CircleShape();

			CircleShape(float radius, Point const& offset = Point());

			void Set(float radius, Point const& offset = Point());

			void FitWorld(World* world) override;

		protected:
			float radius_;
			Point offset_;
			b2CircleShape circle_;
		};

		// ������״
		class KGE_API BoxShape
			: public Shape
		{
		public:
			BoxShape();

			BoxShape(Vec2 const& size, Point const& offset = Point(), float rotation = 0.f);

			void Set(Vec2 const& size, Point const& offset = Point(), float rotation = 0.f);

			void FitWorld(World* world) override;

		protected:
			float rotation_;
			Vec2 box_size_;
			Point offset_;
			b2PolygonShape polygon_;
		};

		// �������״
		class KGE_API PolygonShape
			: public Shape
		{
		public:
			PolygonShape();

			PolygonShape(Vector<Point> const& vertexs);

			void Set(Vector<Point> const& vertexs);

			void FitWorld(World* world) override;

		protected:
			Vector<Point> vertexs_;
			b2PolygonShape polygon_;
		};

		// �߶���״, ���ڱ�ʾһ����
		class KGE_API EdgeShape
			: public Shape
		{
		public:
			EdgeShape();

			EdgeShape(Point const& p1, Point const& p2);

			void Set(Point const& p1, Point const& p2);

			void FitWorld(World* world) override;

		protected:
			Point p_[2];
			b2EdgeShape edge_;
		};

		// ��ʽ��״
		class KGE_API ChainShape
			: public Shape
		{
		public:
			ChainShape();

			ChainShape(Vector<Point> const& vertexs, bool loop = false);

			void Set(Vector<Point> const& vertexs, bool loop = false);

			void FitWorld(World* world) override;

		protected:
			bool loop_;
			Vector<Point> vertexs_;
			b2ChainShape chain_;
		};
	}
}
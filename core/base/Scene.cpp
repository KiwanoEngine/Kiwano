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

#include "Scene.h"
#include "Node.h"

namespace easy2d
{
	Scene::Scene()
		: root_(nullptr)
		, transform_()
	{
	}

	Scene::Scene(Node * root)
		: root_(nullptr)
		, transform_()
	{
		this->SetRoot(root);
	}

	Scene::~Scene()
	{
		if (root_)
		{
			root_->SetParentScene(nullptr);
			root_->Release();
		}
	}

	void Scene::SetRoot(Node * root)
	{
		if (root_ == root)
			return;

		if (root_)
		{
			root_->SetParentScene(nullptr);
			root_->Release();
		}

		if (root)
		{
			root->Retain();
			root->SetParentScene(this);
		}

		root_ = root;
	}

	Node * Scene::GetRoot() const
	{
		return root_;
	}

	void Scene::Draw()
	{
		if (root_)
		{
			root_->Visit();
		}
	}

	void Scene::Dispatch(const MouseEvent & e)
	{
		auto handler = dynamic_cast<MouseEventHandler*>(this);
		if (handler)
		{
			handler->Handle(e);
		}

		if (root_)
		{
			root_->Dispatch(e, false);
		}
	}

	void Scene::Dispatch(const KeyEvent & e)
	{
		auto handler = dynamic_cast<KeyEventHandler*>(this);
		if (handler)
		{
			handler->Handle(e);
		}

		if (root_)
		{
			root_->Dispatch(e, false);
		}
	}

	void Scene::SetTransform(const math::Matrix& matrix)
	{
		transform_ = matrix;

		if (root_)
		{
			root_->dirty_transform_ = true;
		}
	}

	const math::Matrix& Scene::GetTransform() const
	{
		return transform_;
	}
}

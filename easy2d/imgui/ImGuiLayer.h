// Copyright (C) 2019 Nomango

#pragma once

namespace easy2d
{
	E2D_DECLARE_SMART_PTR(ImGuiLayer);

	using ImGuiPipeline = std::function<void()>;

	class ImGuiLayer
		: public Layer
	{
	public:
		ImGuiLayer();

		virtual ~ImGuiLayer();

		// ��� ImGui Ԫ��
		void AddItem(
			ImGuiPipeline const& item,
			String const& name
		);

		// �Ƴ� ImGui Ԫ��
		void RemoveItem(
			String const& name
		);

		// �Ƴ�����Ԫ��
		void RemoveAllItems();

	public:
		void OnMouseButtonDown(int btn, Point const& p) override;
		void OnMouseButtonUp(int btn, Point const& p) override;
		void OnMouseWheel(float wheel) override;

		void OnKeyDown(int key) override;
		void OnKeyUp(int key) override;
		void OnChar(char c) override;

	public:
		void OnUpdate(Duration dt) override;
		void OnRender() override;

		void UpdateMousePos();
		void UpdateMouseCursor();

	protected:
		HWND target_window_;
		Map<String, ImGuiPipeline> pipelines_;
	};
}

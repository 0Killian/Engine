#pragma once

#include "GUI.h"

namespace NGN::GuiComponent
{
	class TreeNode : public GUI::Component
	{
	public:
		enum Flags
		{
			None = 0,
			Leaf = 1 << 0,
			OpenOnDoubleClick = 1 << 1,
		};

		struct Parameters
		{
			const String& Name;
			size_t ID;
			bool Open = true;
			bool DragDropSource = false;
			bool DragDropTarget = false;
			std::optional<std::tuple<std::shared_ptr<Component>, String, std::any>> DragDropPayload = std::nullopt;
			std::function<void()> OnClick = nullptr;
			std::function<void()> OnDoubleClick = nullptr;
			std::function<void(const String& type, std::any data)> OnDragDrop = nullptr;
			Flags Flags = Flags::None;
			List<std::shared_ptr<Component>> Children = {};
		};

		TreeNode(const Parameters& params)
			: Component(params.Name, params.Open, params.DragDropSource, params.DragDropTarget, params.DragDropPayload, params.OnClick, params.OnDoubleClick, params.OnDragDrop)
			, m_ID(params.ID)
			, m_Flags(params.Flags)
			, m_Children(params.Children)
		{}

		void Render(bool canDragDrop = true, size_t width = 0, size_t height = 0) override;

		size_t m_ID;
		Flags m_Flags;
		List<std::shared_ptr<Component>> m_Children;
	};
}
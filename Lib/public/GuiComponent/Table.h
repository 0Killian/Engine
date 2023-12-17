#pragma once

#include "GUI.h"

namespace NGN::GuiComponent
{
	class Table : public GUI::Component
	{
	public:
		union CellConfiguration
		{
			size_t Padding;
			size_t Columns;
		};

		struct Parameters
		{
			const String& Name;
			bool Open = true;
			bool DragDropSource = false;
			bool DragDropTarget = false;
			std::optional<std::tuple<std::shared_ptr<Component>, String, std::any>> DragDropPayload = std::nullopt;
			std::function<void()> OnClick = nullptr;
			std::function<void()> OnDoubleClick = nullptr;
			std::function<void(const String& type, std::any data)> OnDragDrop = nullptr;
			bool UseColumns = false;
			CellConfiguration CellConfiguration;
			size_t CellWidth;
			size_t CellHeight;
			List<std::shared_ptr<Component>> Children = {};
		};

		Table(const Parameters& params)
			: Component(params.Name, params.Open, params.DragDropSource, params.DragDropTarget, params.DragDropPayload, params.OnClick, params.OnDoubleClick, params.OnDragDrop)
			, m_CellConfiguration(params.CellConfiguration)
			, m_CellWidth(params.CellWidth)
			, m_CellHeight(params.CellHeight)
			, m_Children(params.Children)
		{}

		void Render(bool canDragDrop = true, size_t width = 0, size_t height = 0) override;

		bool m_UseColumns = false;
		CellConfiguration m_CellConfiguration;
		size_t m_CellWidth;
		size_t m_CellHeight;
		List<std::shared_ptr<Component>> m_Children;
	};
}
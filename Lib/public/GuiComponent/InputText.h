#pragma once

#include "GUI.h"

namespace NGN::GuiComponent
{
	class InputText : public GUI::Component
	{
	public:
		struct Parameters
		{
			const String& Name;
			const String& Value = "";
			bool Open = true;
			bool DragDropSource = false;
			bool DragDropTarget = false;
			std::optional<std::tuple<std::shared_ptr<Component>, String, std::any>> DragDropPayload = std::nullopt;
			std::function<void()> OnClick = nullptr;
			std::function<void()> OnDoubleClick = nullptr;
			std::function<void(const String& type, std::any data)> OnDragDrop = nullptr;
			std::function<void(const String&)> OnChange = nullptr;
		};

		InputText(const Parameters& params)
			: Component(params.Name, params.Open, params.DragDropSource, params.DragDropTarget, params.DragDropPayload, params.OnClick, params.OnDoubleClick, params.OnDragDrop)
			, m_Value(params.Value)
			, m_OnChange(params.OnChange)
		{}

		virtual void OnChange()
		{
			if (m_OnChange)
				m_OnChange(m_Value);
		}

		void Render(bool canDragDrop = true, size_t width = 0, size_t height = 0) override;

		String m_Value;
		std::function<void(const String&)> m_OnChange;
	};
}
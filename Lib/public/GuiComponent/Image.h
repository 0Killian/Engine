#pragma once

#include "GUI.h"

namespace NGN::GuiComponent
{
	class Image : public GUI::Component
	{
	public:
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
			size_t Width;
			size_t Height;
			void* NativeTexture = nullptr;
		};

		Image(const Parameters& params)
			: Component(params.Name, params.Open, params.DragDropSource, params.DragDropTarget, params.DragDropPayload, params.OnClick, params.OnDoubleClick, params.OnDragDrop)
			, m_Width(params.Width)
			, m_Height(params.Height)
			, m_NativeTexture(params.NativeTexture)
		{}

		void Render(bool canDragDrop = true, size_t width = 0, size_t height = 0) override;

		size_t m_Width;
		size_t m_Height;
		void* m_NativeTexture;
	};
}
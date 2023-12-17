#pragma once

#include "GUI.h"

namespace NGN::GuiComponent
{
	class Vec3 : public GUI::Component
	{
	public:
		struct Parameters
		{
			const String& Name;
			Math::Vec3<float>& Value;
			bool Open = true;
			bool DragDropSource = false;
			bool DragDropTarget = false;
			std::optional<std::tuple<std::shared_ptr<Component>, String, std::any>> DragDropPayload = std::nullopt;
			std::function<void()> OnClick = nullptr;
			std::function<void()> OnDoubleClick = nullptr;
			std::function<void(const String& type, std::any data)> OnDragDrop = nullptr;
			std::function<void(Math::Vec3<float>&)> OnChange = nullptr;
			float Speed = 1.0f;
			float Min = 0.0f;
			float Max = 0.0f;
		};

		Vec3(const Parameters& params)
			: Component(params.Name, params.Open, params.DragDropSource, params.DragDropTarget, params.DragDropPayload, params.OnClick, params.OnDoubleClick, params.OnDragDrop)
			, m_Value(params.Value)
			, m_OnChange(params.OnChange)
			, m_Speed(params.Speed)
			, m_Min(params.Min)
			, m_Max(params.Max)
		{}

		virtual void OnChange()
		{
			if (m_OnChange)
				m_OnChange(m_Value);
		}

		void Render(bool canDragDrop = true, size_t width = 0, size_t height = 0) override;

		Math::Vec3<float>& m_Value;
		std::function<void(Math::Vec3<float>&)> m_OnChange;
		float m_Speed;
		float m_Min;
		float m_Max;
	};
}
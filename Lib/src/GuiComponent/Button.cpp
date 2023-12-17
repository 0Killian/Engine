#include "GuiComponent\Button.h"
#include "imgui.h"
#include <stdexcept>

namespace NGN::GuiComponent
{
	void Button::Render(bool canDragDrop, size_t width, size_t height)
	{
		if (m_NativeTexture)
		{
			if (ImGui::ImageButton(m_Name.GetData(), m_NativeTexture, ImVec2(width, height)))
				OnClick();
		}
		else
		{
			if (ImGui::Button(m_Name.GetData(), ImVec2(width, height)))
				OnClick();
		}

		if (m_DragDropSource)
		{
			if (!canDragDrop)
				throw std::runtime_error("Cannot drag drop here");

			if (ImGui::BeginDragDropSource())
			{
				auto& [comp, type, data] = m_DragDropPayload.value();
				DragDropData dataStruct = { &data, type.GetData() };
				ImGui::SetDragDropPayload("DynData", &dataStruct, sizeof(DragDropData));
				comp->Render(false);
				ImGui::EndDragDropSource();
			}
		}

		if (m_DragDropTarget)
		{
			if (!canDragDrop)
				throw std::runtime_error("Cannot drag drop here");

			if (ImGui::BeginDragDropTarget())
			{
				auto payload = ImGui::AcceptDragDropPayload("DynData");
				if (payload)
				{
					DragDropData data = *(DragDropData*)payload->Data;
					OnDragDrop(data.Type, *data.Data);
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
}
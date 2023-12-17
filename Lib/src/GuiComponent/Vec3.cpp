#include "GuiComponent/Vec3.h"
#include "imgui.h"
#include <stdexcept>

namespace NGN::GuiComponent
{
	void Vec3::Render(bool canDragDrop, size_t width, size_t height)
	{
		if(width != 0)
			ImGui::SetNextItemWidth(width);

		if (ImGui::DragFloat3(m_Name.GetData(), m_Value.data, m_Speed, m_Min, m_Max))
		{
			OnChange();
		}

		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
			OnDoubleClick();
		else if (ImGui::IsItemClicked())
			OnClick();

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
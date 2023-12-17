#include "GuiComponent\Section.h"
#include "imgui.h"
#include <stdexcept>

namespace NGN::GuiComponent
{
	void Section::Render(bool canDragDrop, size_t width, size_t height)
	{
		if(width != 0 && height != 0)
			ImGui::BeginChild(m_Name.GetData(), ImVec2(width, height));
		else
			ImGui::BeginGroup();

		ImGui::Text(m_Name.GetData());
		ImGui::Separator();
		for (auto& child : m_Children)
		{
			child->Render(canDragDrop);
		}

		if (width != 0 && height != 0)
			ImGui::EndChild();
		else
			ImGui::EndGroup();

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
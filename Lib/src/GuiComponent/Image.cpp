#include "GuiComponent/Image.h"
#include "imgui.h"
#include <stdexcept>

namespace NGN::GuiComponent
{
	void Image::Render(bool canDragDrop, size_t width, size_t height)
	{
		ImGui::Image(m_NativeTexture, ImVec2(static_cast<float>(width > 0 ? width : m_Width), static_cast<float>(height > 0 ? height : m_Height)));
		
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
#include "GuiComponent\Table.h"
#include "imgui.h"
#include <stdexcept>

namespace NGN::GuiComponent
{
	void Table::Render(bool canDragDrop, size_t width, size_t height)
	{
		size_t columns = m_CellConfiguration.Columns;
		if (!m_UseColumns)
		{
			size_t padding = m_CellConfiguration.Padding;
			size_t width = m_CellWidth + padding;
			size_t height = m_CellHeight + padding;
			float panelWidth = width > 0 ? static_cast<float>(width) : ImGui::GetContentRegionAvail().x;
			columns = static_cast<size_t>(std::floor(panelWidth / width));

			if (columns == 0)
				columns = 1;
		}

		if (ImGui::BeginTable(m_Name.GetData(), columns, ImGuiTableFlags_None, ImVec2(static_cast<float>(width), static_cast<float>(height))))
		{
			for (auto& child : m_Children)
			{
				ImGui::TableNextColumn();
				child->Render(canDragDrop, m_UseColumns ? 0 : width, m_UseColumns ? 0 : height);
			}
			ImGui::EndTable();
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
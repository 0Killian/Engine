#include "GuiComponent/TreeNode.h"
#include "imgui.h"
#include <stdexcept>

namespace NGN::GuiComponent
{
	void TreeNode::Render(bool canDragDrop, size_t width, size_t height)
	{
		ImGui::PushID(m_ID);
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;

		if (m_Flags & Flags::Leaf)
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}

		if (m_Flags & Flags::OpenOnDoubleClick)
		{
			flags |= ImGuiTreeNodeFlags_OpenOnDoubleClick;
		}

		if(width != 0)
			ImGui::SetNextItemWidth(static_cast<float>(width));
		bool nodeOpen = ImGui::TreeNodeEx(m_Name.GetData(), flags);

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

		if (nodeOpen)
		{
			for (auto& child : m_Children)
			{
				child->Render(canDragDrop);
			}

			ImGui::TreePop();
		}

		ImGui::PopID();
	}
}
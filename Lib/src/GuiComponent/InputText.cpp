#include "GuiComponent\InputText.h"
#include "imgui.h"
#include <stdexcept>

namespace NGN::GuiComponent
{
	struct InputTextCallbackData
	{
		String* buffer;
		ImGuiInputTextCallback ChainCallback;
		void* ChainCallbackUserData;
	};

	static int InputTextCallback(ImGuiInputTextCallbackData* data)
	{
		InputTextCallbackData* callbackData = (InputTextCallbackData*)data->UserData;
		if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
		{
			// Resize string callback
			// If for some reason within this callback the string needs to be resized again, it will be called again with the new length.
			String* str = callbackData->buffer;
			IM_ASSERT(data->Buf == str->GetData());
			str->Resize(data->BufTextLen);
			data->Buf = (char*)str->GetData();
		}
		else if (callbackData->ChainCallback)
		{
			// Forward to user callback, if any
			data->UserData = callbackData->ChainCallbackUserData;
			return callbackData->ChainCallback(data);
		}

		return 0;
	}

	void InputText::Render(bool canDragDrop, size_t width, size_t height)
	{
		InputTextCallbackData callbackData;
		callbackData.buffer = &m_Value;
		callbackData.ChainCallback = nullptr;
		callbackData.ChainCallbackUserData = nullptr;

		if(width != 0)
			ImGui::SetNextItemWidth(width);
		if (ImGui::InputText(m_Name.GetData(), m_Value.GetData(), m_Value.GetCapacity() + 1, ImGuiInputTextFlags_CallbackResize, InputTextCallback, (void*)&callbackData))
		{
			m_Value.Resize(strlen(m_Value.GetData()));
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
#include "GUI.h"
#include "Application.h"
#include "imgui.h"

namespace NGN
{
	GUI& GUI::Get()
	{
		return Application::Get().GetGUI();
	}

	GUI::GUI()
	{
		m_Context = ImGui::CreateContext();
		ImGui::SetCurrentContext(m_Context);
		ImGui::StyleColorsDark();
		
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	}

	void GUI::BeginFrame()
	{
		ImGui::NewFrame();
	}

	void GUI::EndFrame()
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	void GUI::StartWindow(const char* name, bool& open)
	{
		ImGui::Begin(name, &open);
	}

	void GUI::StartWindow(const char* name)
	{
		ImGui::Begin(name);
	}

	void GUI::EndWindow()
	{
		ImGui::End();
	}

	void GUI::PushID(uint32_t id)
	{
		ImGui::PushID(id);
	}

	void GUI::PopID()
	{
		ImGui::PopID();
	}

	bool GUI::TreeNode(const char* label, TreeNodeFlags flags)
	{
		ImGuiTreeNodeFlags imguiFlags = 0;
		if (flags & TreeNodeFlags::Leaf)
					imguiFlags |= ImGuiTreeNodeFlags_Leaf;

		return ImGui::TreeNodeEx(label, imguiFlags);
	}

	void GUI::TreePop()
	{
		ImGui::TreePop();
	}

	bool GUI::IsItemClicked()
	{
		return ImGui::IsItemClicked();
	}

	void GUI::Section(const char* label)
	{
		ImGui::Text(label);
		ImGui::Separator();
	}

	bool GUI::Vec3(const char* label, Math::Vec3<float>& vec, float min, float max)
	{
		return ImGui::DragFloat3(label, vec.data, 1.0f, min, max);
	}
}
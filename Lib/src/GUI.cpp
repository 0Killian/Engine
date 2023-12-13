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

	void GUI::SampleWindow()
	{
		if(m_ShowDemoWindow)
			ImGui::ShowDemoWindow(&m_ShowDemoWindow);
	}

	void GUI::EndFrame()
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}
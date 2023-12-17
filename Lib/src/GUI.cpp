#include "GUI.h"
#include "Application.h"
#include "imgui.h"
#include "imgui_internal.h"

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

	void GUI::Init(const HashMap<String, std::shared_ptr<Window>>& windows)
	{
		m_Windows = windows;
	}

	void GUI::BeginFrame()
	{
		ImGui::NewFrame();
		m_NextID = 0;

		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;//;ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		//window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::Begin("DockSpace", nullptr, window_flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (!m_DockSpaceInitialized)
		{
			m_DockSpaceInitialized = true;

			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			ImGuiID left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
			ImGuiID right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.2f, nullptr, &dockspace_id);
			ImGuiID top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.2f, nullptr, &dockspace_id);
			ImGuiID bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.2f, nullptr, &dockspace_id);

			for (const auto& [name, window] : m_Windows)
			{
				if (window->DockSide == Side::Bottom)
				{
					ImGui::DockBuilderDockWindow(name.GetData(), bottom);
				}
				else if (window->DockSide == Side::Left)
				{
					ImGui::DockBuilderDockWindow(name.GetData(), left);
				}
				else if (window->DockSide == Side::Right)
				{
					ImGui::DockBuilderDockWindow(name.GetData(), right);
				}
				else if (window->DockSide == Side::Top)
				{
					ImGui::DockBuilderDockWindow(name.GetData(), top);
				}
				else if (window->DockSide == Side::Tab)
				{
					throw std::runtime_error("Unimplemented");
				}
				else if (window->DockSide == Side::Viewport)
				{
					ImGui::DockBuilderDockWindow(name.GetData(), dockspace_id);
				}
			}

			ImGui::DockBuilderFinish(dockspace_id);
		}
	}

	void GUI::EndFrame()
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	void GUI::Render(HashMap<String, std::shared_ptr<Window>>& additionalWindows)
	{
		for (auto& [name, window] : m_Windows)
		{
			RenderWindow(name, *window);
		}

		for (auto& [name, window] : additionalWindows)
		{
			if(m_Windows.ContainsKey(name))
				throw std::runtime_error((String("Window with name ") + name + " already exists").GetData());

			if(window->DockSide != std::nullopt)
				throw std::runtime_error("Additional windows cannot be docked");

			RenderWindow(name, *window);
		}

		ImGui::End();
	}

	void GUI::RenderWindow(const String& name, Window& window)
	{
		if(!window.Open)
			return;

		if (window.X.has_value() && window.Y.has_value())
		{
			ImGui::SetNextWindowPos(ImVec2(window.X.value(), window.Y.value()));
		}

		if (window.Width.has_value() && window.Height.has_value())
		{
			ImGui::SetNextWindowSize(ImVec2(window.Width.value(), window.Height.value()));
		}

		ImGuiWindowFlags flags = ImGuiWindowFlags_None;
		int styleVarCount = 0;

		if (window.Flags & Window::DockSpace)
		{
			throw std::runtime_error("Unimplemented");
		}
		else
		{
			if (window.Flags & Window::NoDecoration)
			{
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

				styleVarCount = 2;
				flags |= ImGuiWindowFlags_NoDecoration;
			}

			if (window.Flags & Window::NoMove)
			{
				flags |= ImGuiWindowFlags_NoMove;
			}

			if (window.Flags & Window::NoResize)
			{
				flags |= ImGuiWindowFlags_NoResize;
			}

			if (window.Flags & Window::NoBringToFrontOnFocus)
			{
				flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			}

			if (window.Flags & Window::NoNavFocus)
			{
				flags |= ImGuiWindowFlags_NoNavFocus;
			}

			if (window.Flags & Window::NoBackground)
			{
				flags |= ImGuiWindowFlags_NoBackground;
				ImGui::SetNextWindowBgAlpha(0.0f);
				ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			}
		}

		ImGui::Begin(
			name.GetData(),
			nullptr,
			flags);

		ImGui::PopStyleVar(styleVarCount);
		if(window.Flags & Window::NoBackground)
			ImGui::PopStyleColor();

		if (window.Flags & Window::DockSpace)
		{
		}

		for (auto& component : window.Components)
		{
			component->Render();
		}

		window.ComputedX = ImGui::GetWindowPos().x;
		window.ComputedY = ImGui::GetWindowPos().y;
		window.ComputedWidth = ImGui::GetWindowWidth();
		window.ComputedHeight = ImGui::GetWindowHeight();

		ImGui::End();
	}
}
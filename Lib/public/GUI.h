#pragma once

struct ImGuiContext;

namespace NGN
{
	class GUI
	{
	public:
		static GUI& Get();

		GUI();

		void BeginFrame();
		void EndFrame();
		void SampleWindow();

	private:
		ImGuiContext* m_Context;
		bool m_ShowDemoWindow = true;
	};
}
#pragma once

#include <stdint.h>

#include "Math/Vec3.h"

struct ImGuiContext;

namespace NGN
{
	class GUI
	{
	public:
		enum TreeNodeFlags
		{
			Leaf = 1 << 0,
		};

		static GUI& Get();

		GUI();

		void BeginFrame();
		void EndFrame();

		void StartWindow(const char* title, bool& open);
		void StartWindow(const char* title);
		void EndWindow();

		void PushID(uint32_t id);
		void PopID();

		bool TreeNode(const char* label, TreeNodeFlags flags = static_cast<TreeNodeFlags>(0));
		void TreePop();
		bool IsItemClicked();

		void Section(const char* label);

		bool Vec3(const char* label, Math::Vec3<float>& vec, float min = 0.0f, float max = 0.0f);

	private:
		ImGuiContext* m_Context;
	};
}